#include <iostream>
#include <filesystem/FileManager.h>
#include <lsp-tools/utils.h>
#include "CompletionProvider.h"

#include "lsp-tools/definitions.h"
#include "lsp-tools/messaging.h"

void sendNullResult (Id &id) {
    INIT_WRITER
    ADD_ID(id);
    ADD_NULL_RESULT();
    SEND_MESSAGE
}

void CompletionProvider::registerCapabilities (Init::ServerCapabilities &capabilities) {
    if (!capabilities.completionProvider) capabilities.completionProvider = Init::CompletionOptions {};

    /*
     * \ - command
     * @ - bib item
     * ! - magic comment
     *
     */
    capabilities.completionProvider->triggerCharacters = { "\\", "@", "!" };
}

vector<TSNode> getNodeTreeForPoint (TSNode rootNode, TSPoint point) {
    vector<TSNode> nodes {};

    TSNode node = ts_node_named_descendant_for_point_range(rootNode, point, point);

    while (!ts_node_eq(rootNode, node)) {
        nodes.push_back(node);
        node = ts_node_parent(node);
    }

    return nodes;
}

void printNodeTree (vector<TSNode> &nodes) {
    std::cerr << "Nodes at cursor: ";
    for (auto node : nodes) {
        std::cerr << ts_node_type(node) << ", ";
    }
    std::cerr << "\n";
}

void addMathCompletions (CompletionList *completions, Range &range) {
    string prefix { "math" };
    string body { "complicated math" };
    completions->addSnippet(prefix, body, range);
}

Range getPrefixRange (File &file, TSPoint &tsPoint) {
    auto endPoint = Point { tsPoint.row, tsPoint.column >> 1 };
    auto startPoint = endPoint;

    auto lineStart = file.textInRange({ { endPoint.row, 0 }, endPoint });


    auto rend = lineStart.rend();
    for (auto ritr = lineStart.rbegin(); ritr != rend; ritr++, startPoint.column--) {
        auto c = *ritr;
        if (!(iswalnum(c))) {
            break;
        }
    }

    return Range { startPoint, endPoint };
}

CompletionList* getLatexCompletionsForFileAndPoint (File &file, TSPoint &point) {
    auto *completions = new CompletionList {};

    vector<TSNode> nodeTree = getNodeTreeForPoint(ts_tree_root_node(file.getParseTree()), point);

    printNodeTree(nodeTree);

    for (auto node : nodeTree) {
        if (strcmp("math", ts_node_type(node)) == 0) {
            std::cerr << "adding math snippet\n";
            Range replaceRange = getPrefixRange(file, point);
            addMathCompletions(completions, replaceRange);
        }
    }

    return completions;
}

void CompletionProvider::run (Id &id, optional<Value> &paramsOpt) {
    std::cerr << "Getting completions\n";
    if (!paramsOpt) {
        sendError(&id, ResponseHandler::ErrorCode::InvalidParams, "Missing params for autocomplete request");
        return;
    }

    Value &params = *paramsOpt;

    Value &textDocumentIdentifier = params["textDocument"];
    string uri = textDocumentIdentifier["uri"].GetString();

    File *file = FileManager::get(uri);
    if (file == nullptr) {
        std::cerr << "!!! Null file !!!\n";
        sendNullResult(id);
        return;
    }

    TSPoint point = getTSPointFromJSON(params["position"]);

    CompletionList *completions { nullptr };

    switch (file->type) {
        case File::Type::Tex:
            completions = getLatexCompletionsForFileAndPoint(*file, point);
            break;
        case File::Type::Bib:
            // TODO
        default: {}
    }

    if (completions == nullptr) {
        sendNullResult(id);
        return;
    }

    INIT_WRITER
    ADD_ID(id);
    writer.Key("result"); completions->reflect(writer);
    SEND_MESSAGE

    delete completions; // TODO: Manage and cache completions elsewhere
}

void CompletionList::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("isIncomplete"); writer.Bool(isIncomplete);
    writer.Key("items"); writer.StartArray();
    for (auto &item : items) {
        item.reflect(writer);
    }
    writer.EndArray();
    writer.EndObject();
}

void CompletionList::addSnippet (string &prefix, string &body, Range &range) {
    CompletionItem snippet {};
    snippet.label = prefix;

    snippet.textEdit.newText = body;
    snippet.textEdit.range = range;

    items.emplace_back(snippet);
}

void CompletionItem::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("label"); writer.String(label);
    writer.Key("kind"); writer.Int((int) kind);
    if (!detail.empty()) {
        writer.Key("detail"); writer.String(detail);
    }
    if (!documentation.empty()) {
        writer.Key("documentation"); writer.String(documentation); // TODO: Support MarkupContent
    }
    if (deprecated) {
        writer.Key("deprecated"); writer.Bool(deprecated);
    }
    writer.Key("textEdit"); textEdit.reflect(writer);
    writer.EndObject();
}

void TextEdit::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("range"); ADD_RANGE(range);
    writer.Key("newText"); writer.String(newText);
    writer.EndObject();
}
