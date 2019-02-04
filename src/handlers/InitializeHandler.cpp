#include <iostream>

#include "lsp-tools/definitions.h"
#include "InitializeHandler.h"
#include "lsp-tools/HandlerManager.h"
#include "lsp-tools/messaging.h"


void registerAllHandlerCapabilities (Init::ServerCapabilities &capabilities) {
    HandlerManager::getInstance()->registerCapabilities(capabilities);
}

void InitializeHandler::run (Id &id, optional<Value> &params) {
    INIT_WRITER

    ADD_ID(id);

    writer.Key("result"); writer.StartObject();

    writer.Key("capabilities");

    Init::ServerCapabilities capabilities {};

    registerAllHandlerCapabilities(capabilities);

    capabilities.reflect(writer);

    writer.EndObject();

    SEND_MESSAGE
}

void InitializeHandler::registerCapabilities (Init::ServerCapabilities &capabilities) {
    capabilities.referencesProvider = true;
    capabilities.definitionProvider = true;
    capabilities.documentHighlightProvider = true;
}


