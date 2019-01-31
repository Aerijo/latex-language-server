#include <tree_sitter/parser.h>
#include "BibIndexer.h"

#include <stack>
#include <lsp-tools/messaging.h>

extern "C" {
    TSLanguage *tree_sitter_biber();
}

using std::stack;

void makeLowerCase (u16string &input) {
    // TODO: Full unicode support (but ASCII should be adequate for bibtex compliant sources)
    auto end = input.end();
    for (auto itr = input.begin(); itr != end; itr++) {
        auto &c = *itr;

        if ('A' <= c && c <= 'Z') {
            c += 32;
        }
    }
}

void BibEntryIssue::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("range"); ADD_TS_RANGE(range);
    writer.Key("severity"); writer.Int(static_cast<int>(severity));
    writer.Key("code"); writer.Int((int) code);
    writer.Key("source"); writer.String(source);
    writer.Key("message"); writer.String(message);
    writer.EndObject();
}

void BibIndexer::addLint (const TSNode &node, Severity sev, int code, const string &msg) {
    TSPoint start = ts_node_start_point(node);
    TSPoint end = ts_node_end_point(node);
    issues.emplace_back(BibEntryIssue { { start, end }, sev, code, msg });
}

void BibIndexer::addError (const TSNode &node, Error error, const string &msg) {
    addLint(node, Severity::Error, (int) error, msg);
}

void BibIndexer::addWarning (const TSNode &node, Warning warning, const string &msg) {
    addLint(node, Severity::Warning, (int) warning, msg);
}

void BibIndexer::addInfo (const TSNode &node, Info info, const string &msg) {
    addLint(node, Severity::Info, (int) info, msg);
}

void BibIndexer::lintErrors (TSNode rootNode) {
    stack<TSNode> path;

    path.push(rootNode);

    while (!path.empty()) {
        const TSNode node = path.top(); path.pop();

        if (ts_node_has_error(node)) {
            TSSymbol symbol = ts_node_symbol(node);
            if (symbol == ts_builtin_sym_error) {
                addError(node, Error::Generic, "Error!");
            } else {
                uint32_t numChildren = ts_node_child_count(node);
                for (uint32_t i = 0; i < numChildren; i++) {
                    TSNode child = ts_node_child(node, i);
                    path.push(child);
                }
            }
        }
    }
}

void BibIndexer::completeIndex () {
    clearCached();
    assert (file != nullptr);

    const TSTree *tree = file->getParseTree();
    assert (tree != nullptr);

    std::cerr << "reindexing bib file " << file->getPath() << "\n";

    TSNode rootNode = ts_tree_root_node(tree);

    lintErrors(rootNode);

    lintFile(rootNode);

    std::cerr << "found " << issues.size() << " issues...\n";

    publishErrors();

    std::cerr << "published errors\n";
}

void BibIndexer::partialReindex (Point &changesStart) {

}

void BibIndexer::clearCached () {
    entries.clear();
    strings.clear();
    issues.clear();
    keys.clear();
}

void BibIndexer::publishErrors () {
    INIT_WRITER

    writer.Key("method"); writer.String("textDocument/publishDiagnostics");

    writer.Key("params"); writer.StartObject();
        writer.Key("uri"); writer.String(file->getPath());
        writer.Key("diagnostics"); writer.StartArray();
            for (auto issue : issues) {
                issue.reflect(writer);
            }
        writer.EndArray();
    writer.EndObject();

    SEND_MESSAGE
}

// Copied from tree-sitter-biber/src/parser.c
// TODO: Automate this somehow (needs to be compile time constant for switch statements)
enum Sym {
    SymComment = 1,
    SymJunk = 25,
    SymEntry = 30,
    SymKey = 31,
    SymName = 32,
    SymField = 33,
    SymValue = 34,
    SymString = 36
};

void BibIndexer::lintFile (TSNode rootNode) {
    std::cerr << "Fully linting bib file " << file->getPath() << "\n";

    if (style == nullptr) {
        std::cerr << "Cannot analyse bib file without style!\n"; // though can do keys and such...
    }

    auto num_children = ts_node_child_count(rootNode);
    for (uint32_t i = 0; i < num_children; i++) {
        TSNode child = ts_node_child(rootNode, i);

        switch (ts_node_symbol(child)) {
            case SymEntry:
                lintEntry(child);
            default: {}
        }
    }
}

bool BibIndexer::getEntryName (u16string &entryName, TSNode &node, uint32_t &index, uint32_t childCount) {
    for (; index < childCount; index++) {
        TSNode child = ts_node_named_child(node, index);

        switch (ts_node_symbol(child)) {
            case SymName:
                entryName = file->textForNode(child);
                makeLowerCase(entryName);
                return true;
            case SymComment:
                // TODO: Complain about comments inside the entry block
                break;
            default:
                addError(node, Error::MissingEntryName, "Could not find entry name");
                return false;
        }
    }

    return false;
}

void BibIndexer::lintEntry (TSNode &entryNode) {
    u16string entryName {};

    vector<u16string> observedFields {};

    uint32_t childCount = ts_node_named_child_count(entryNode);
    uint32_t i = 0;

    bool foundEntryName = getEntryName(entryName, entryNode, i, childCount);
    if (!foundEntryName) return;

    if (entryName.empty()) {
        TSNode entryNameNode = ts_node_child(entryNode, 0);
        addError(entryNameNode, Error::MissingEntryName, "No entry name");
        return;
    }

    std::optional<Style::Entry *> oentry = style->getEntry(entryName);

    if (!oentry) {
        TSNode entryNameNode = ts_node_named_child(entryNode, i);
        addWarning(entryNameNode, Warning::UnknownEntry, "Entry " + UtfHandler().utf16to8(entryName) + " is unexpected");
    }
}
