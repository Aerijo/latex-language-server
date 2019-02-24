#include <iostream>
#include <algorithm>
#include <filesystem/FileManager.h>
#include <lsp-tools/utils.h>
#include "CompletionProvider.h"

#include "lsp-tools/definitions.h"
#include "lsp-tools/messaging.h"

#include <pcre.h>

#define PREFIX_MAX_LENGTH 100

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

/*
 * The path returned is going most -> least specific, and excludes the root node.
 */
vector<TSNode> getNodePathForPoint (TSNode rootNode, TSPoint point) {
    vector<TSNode> nodes {};

    TSNode node = ts_node_named_descendant_for_point_range(rootNode, point, point);

    while (!ts_node_eq(rootNode, node)) {
        nodes.push_back(node);
        node = ts_node_parent(node);
    }

    return nodes;
}

void printNodes (vector<TSNode> &nodes) {
    std::cerr << "Nodes at cursor: ";
    for (auto node : nodes) {
        std::cerr << ts_node_type(node) << ", ";
    }
    std::cerr << "\n";
}

void addMathCompletions (CompletionList *completions, u16string &prefix, Range prefixRange) {
    string prefixz { "math" };
    string body { "complicated $1 math" };
    completions->addSnippet(prefixz, body, prefixRange);
}



u16string getEnvPrefix (File &file, TSNode envNameNode, TSPoint tsPoint) {
    return u"";
}

u16string getCitationPrefix (File &file, TSNode citationKeyNode, TSPoint tsPoint) {
    return u"";
}

/*
 * Gets all letters back from the cursor to a \, !, or @.
 *
 * Alternatively gets $ or $$ if they are the first characters,
 * and will try all non space characters terminated with @ for a
 * citation key
 */
u16string getTextPrefix (File &file, TSPoint tsPoint, PrefixType *type) {
    auto cursorPosition = Point { tsPoint.row, tsPoint.column >> 1 };

    // we only check the last PREFIX_MAX_LENGTH characters at most
    unsigned startCol { 0 };
    if (cursorPosition.column > PREFIX_MAX_LENGTH) {
        startCol = cursorPosition.column - PREFIX_MAX_LENGTH;
    }
    auto candidateText = file.textInRange({ { cursorPosition.row, startCol }, cursorPosition });
    auto i = candidateText.size();
    if (i == 0) { return u""; }
    auto c = candidateText[--i];

    if (c == '$') {
        *type = PrefixType::MathShift;
        if (i > 0 && candidateText[--i] == '$') {
            return u"$$";
        }
        return u"$";
    }

    for (; i >= 0; --i) {
        c = candidateText[i];
        if (iswalpha(c)) continue;
        switch (c) {
            case ' ':
                return {};
            case '\\':
                *type = PrefixType::TextCommand;
                return candidateText.substr(i, string::npos);
            case '@':
                *type = PrefixType::Citation;
                return candidateText.substr(i, string::npos);
            case '!':
                *type = PrefixType::Magic;
                return candidateText.substr(i, string::npos);
            default: {}
        }

        --i;
        break;
    }

    for (; i >= 0; --i) {
        c = candidateText[i];
        switch (c) {
            case ' ':
                return {};
            case '@':
                *type = PrefixType::Citation;
                return candidateText.substr(i, string::npos);
            default: {}
        }
    }

    return {};
}

bool isEnvironment (const char *type) {
    size_t i = 0;
    while (type[i] != '\0') i++;

    if (i <= 4) return false;

    return // ends with _env
        type[--i] == '_' &&
        type[--i] == 'v' &&
        type[--i] == 'n' &&
        type[--i] == 'e';
}

PrefixType identifyCompletionType (vector<TSNode> &nodePath) {
    for (auto node : nodePath) {
        const char *type = ts_node_type(node);
        if (std::strcmp(type, "cite") == 0) {
            return PrefixType::Citation;
        } else if (isEnvironment(type)) {
            return PrefixType::Env;
        } else if (std::strcmp(type, "math") == 0) {
            return PrefixType::MathCommand;
        } else if (std::strcmp(type, "use") == 0) {
            return PrefixType::Package;
        } else if (std::strcmp(type, "ref") == 0) {
            return PrefixType::Reference;
        }
    }

    return PrefixType::None;
}

/*
 * steps:
 * 1. Identify completion type from node path at cursor (can find standard math, citation, reference, env, package here)
 * 2. If text:
 *    - Get prefix. This is (in reverse) word characters terminated by \ or !, or extended word characters terminated by @
 */
CompletionList* getLatexCompletionsForFileAndPoint (File &file, TSPoint &point) {
    auto *completions = new CompletionList {};

    vector<TSNode> nodePath = getNodePathForPoint(ts_tree_root_node(file.getParseTree()), point);
    printNodes(nodePath);

    PrefixType type = identifyCompletionType(nodePath);
    switch (type) {

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
    writer.Key("insertTextFormat"); writer.Int((int) format);
    writer.Key("textEdit"); textEdit.reflect(writer);
    writer.EndObject();
}

void TextEdit::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("range"); ADD_RANGE(range);
    writer.Key("newText"); writer.String(newText);
    writer.EndObject();
}
