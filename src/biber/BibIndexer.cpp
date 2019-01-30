#include <tree_sitter/parser.h>
#include "BibIndexer.h"

#include <stack>
#include <lsp-tools/messaging.h>

extern "C" {
    TSLanguage *tree_sitter_biber();
}

using std::stack;

void BibEntryIssue::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("range"); ADD_TS_RANGE(range);
    writer.Key("severity"); writer.Int(static_cast<int>(severity));
    writer.Key("code"); writer.Int((int) code);
    writer.Key("source"); writer.String(source);
    writer.Key("message"); writer.String(message);
    writer.EndObject();
}

void BibIndexer::addError (const TSNode &node, Error error, const string &msg) {
    TSPoint start = ts_node_start_point(node);
    TSPoint end = ts_node_end_point(node);

    issues.emplace_back(BibEntryIssue {
                                { start, end },
                                Severity::Error,
                                error,
                                msg
                        });
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

enum Sym {
    Entry = 30,
};

void BibIndexer::lintFile (TSNode rootNode) {
    std::cerr << "Fully linting bib file " << file->getPath() << "\n";

    auto num_children = ts_node_child_count(rootNode);
    for (uint32_t i = 0; i < num_children; i++) {
        TSNode child = ts_node_child(rootNode, i);

        switch (ts_node_symbol(child)) {
            case Sym::Entry:
                lintEntry(child);
            default: {}
        }
    }
}

void BibIndexer::lintEntry (TSNode &entry) {
    addError(entry, Error::Entry, "Found entry");
}
