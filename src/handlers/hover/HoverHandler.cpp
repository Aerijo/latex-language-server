#include <iostream>

#include "HoverHandler.h"

void HoverHandler::registerCapabilities (Init::ServerCapabilities &capabilities) {
    capabilities.hoverProvider = true;
}

void HoverHandler::run (Id &id, optional<Value> &params) {
    std::cerr << "Gathering hover info...\n";
}
