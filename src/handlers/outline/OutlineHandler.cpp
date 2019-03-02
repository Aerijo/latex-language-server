#include <iostream>
#include <filesystem/File.h>
#include <filesystem/FileManager.h>

#include "../../lsp-tools/messaging.h"
#include "OutlineHandler.h"

#include "../util.h"

#include "latexOutline.cpp"

void OutlineHandler::registerCapabilities (Init::ServerCapabilities &capabilities) {
    std::cerr << "registering\n";

    capabilities.documentSymbolProvider = true;
}

vector<DocumentSymbol> getOutlineForBibFile (File &file) {
    vector<DocumentSymbol> symbols {};
    return symbols;
}

void OutlineHandler::run (Id &id, optional<Value> &paramsOpt) {
    std::cerr << "Getting outline...\n";

    if (!paramsOpt) {
        sendError(&id, ResponseHandler::ErrorCode::InvalidParams, "missing params");
        return;
    }

    Value &params = *paramsOpt; // TODO: Make function

    Value &textDocumentIdentifier = params["textDocument"];
    string uri = textDocumentIdentifier["uri"].GetString();

    File *file = FileManager::get(uri);
    if (file == nullptr) {
        std::cerr << "!!! Null file !!!\n";
        sendNullResult(id);
        return;
    }

    vector<DocumentSymbol> symbols;
    switch (file->type) {
        case File::Type::Tex:
            symbols = getOutlineForLatexFile(*file);
            break;
        case File::Type::Bib:
            symbols = getOutlineForBibFile(*file);
            break;
        default:
            sendNullResult(id);
            return;
    }

    INIT_WRITER
    ADD_ID(id);
    writer.Key("result");
    writer.StartArray();
    for (auto &symbol : symbols) {
        symbol.reflect(writer);
    }
    writer.EndArray();
    SEND_MESSAGE
}

void DocumentSymbol::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("name"); writer.String(name);
    if (!detail.empty()) {
        writer.Key("detail"); writer.String(detail);
    }
    writer.String("kind"); writer.Int((int) kind);
    if (deprecated) {
        writer.Key("deprecated"); writer.Bool(deprecated);
    }
    writer.Key("range"); ADD_RANGE(range);
    writer.Key("selectionRange"); ADD_RANGE(selectionRange);

    if (!children.empty()) {
        writer.Key("children");
        writer.StartArray();
        for (auto &child : children) {
            child.reflect(writer);
        }
        writer.EndArray();
    }
    writer.EndObject();
}
