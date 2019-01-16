#ifndef LATEX_LANGUAGE_SERVER_INITIALIZE_H
#define LATEX_LANGUAGE_SERVER_INITIALIZE_H

#include <string>
#include <vector>
#include <optional>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

#include "definitions.h"
#include "Handler.h"

using std::string;
using std::vector;
using std::optional;

using rapidjson::Writer;
using rapidjson::StringBuffer;
using rapidjson::Value;

void registerAllHandlerCapabilities (Init::ServerCapabilities &capabilities);

class InitializeHandler : public RequestHandler {
public:
    InitializeHandler () : RequestHandler ("initialize") {};

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_INITIALIZE_H
