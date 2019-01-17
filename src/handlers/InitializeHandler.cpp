#include <iostream>

#include "definitions.h"
#include "InitializeHandler.h"
#include "HandlerManager.h"
#include "messaging.h"


void registerAllHandlerCapabilities (Init::ServerCapabilities &capabilities) {
    HandlerManager::getInstance()->registerCapabilities(capabilities);
}

void InitializeHandler::run (Id &id, optional<Value> &params) {
    std::cerr << "handling initialisation request...\n";

    INIT_WRITER

    writer.Key("id"); id.writeId(writer);

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


