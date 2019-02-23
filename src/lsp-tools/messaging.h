#ifndef LATEX_LANGUAGE_SERVER_MESSAGING_H
#define LATEX_LANGUAGE_SERVER_MESSAGING_H

#include <string>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

#include "definitions.h"
#include "Handler.h"

using std::string;

using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::Value;

void sendMessage (StringBuffer &buffer);

void sendMessage (Document &message);

void cancelRequest (Id &id);

void sendError (Id *id, ResponseHandler::ErrorCode code, const string message, Value *data = nullptr);

void sendError (Id *id, ResponseHandler::ErrorCode code, const char *message, Value *data = nullptr);

void gotoNextHeader ();

size_t readHeaders ();

Document getMessage ();

void launchStdinLoop ();

void launchStdoutLoop ();

#endif //LATEX_LANGUAGE_SERVER_MESSAGING_H
