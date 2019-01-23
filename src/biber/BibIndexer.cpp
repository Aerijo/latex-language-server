#include <tree_sitter/parser.h>
#include "BibIndexer.h"

#include <stack>
#include <lsp-tools/messaging.h>

using std::stack;

void reflectTSPoint (StringWriter &writer, TSPoint &point) {
    writer.StartObject();
    writer.Key("line"); writer.Uint64(point.row);

    // NOTE: The column is in bytes, so we translate to UTF16. This should be fully LSP compliant,
    //  as it also considers multibyte UTF16 characters to also take up multiple columns
    writer.Key("character"); writer.Uint64(point.column >> 1);
    writer.EndObject();
}

void reflectTSRange (StringWriter &writer, TSRange &range) {
    writer.StartObject();
    writer.Key("start"); reflectTSPoint(writer, range.start_point);
    writer.Key("end"); reflectTSPoint(writer, range.end_point);
    writer.EndObject();
}

void BibEntryIssue::reflect (StringWriter &writer) {
    writer.StartObject();

    writer.Key("range"); reflectTSRange(writer, range);
    writer.Key("severity"); writer.Int(static_cast<int>(severity));
    writer.Key("code"); writer.Int(code);
    writer.Key("source"); writer.String(source);
    writer.Key("message"); writer.String(message);

    writer.EndObject();
}

void BibIndexer::addError (const TSNode &node, Error error, const string &msg) {
    TSPoint start = ts_node_start_point(node);
    TSPoint end = ts_node_end_point(node);

    std::cerr << "Starts at [" << start.row << "," << start.column << "]\n";

    BibEntryIssue issue {
            { start, end },
            Severity::Error,
            error,
            msg
    };

    issues.emplace_back(issue);
}

void BibIndexer::lintErrors (TSNode rootNode) {
    stack<TSNode> path;

    path.push(rootNode);

    while (!path.empty()) {
        const TSNode node = path.top(); path.pop();

        if (ts_node_has_error(node)) {
            TSSymbol symbol = ts_node_symbol(node);
            if (symbol == ts_builtin_sym_error) {
                addError(node, Generic, "Error!");
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

    std::cerr << "finding errors...\n";

    lintErrors(rootNode);

    std::cerr << "found " << issues.size() << " errors...\n";

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
