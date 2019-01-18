#ifndef LATEX_LANGUAGE_SERVER_INITIALIZE_H
#define LATEX_LANGUAGE_SERVER_INITIALIZE_H

#include <optional.h>

#include <rapidjson/document.h>

#include "definitions.h"
#include "Handler.h"


using rapidjson::Value;

void registerAllHandlerCapabilities (Init::ServerCapabilities &capabilities);

class InitializeHandler : public RequestHandler {
public:
    InitializeHandler () : RequestHandler ("initialize") {};

    void registerCapabilities (Init::ServerCapabilities &capabilities) override;

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_INITIALIZE_H
