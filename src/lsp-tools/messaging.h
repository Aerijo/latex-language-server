#ifndef LATEX_LANGUAGE_SERVER_MESSAGING_H
#define LATEX_LANGUAGE_SERVER_MESSAGING_H

#include <iostream>
#include <rapidjson/writer.h>

#include "definitions.h"
#include "Handler.h"

void sendMessage (StringBuffer &buffer);

void sendMessage (Document &message);

void cancelRequest (Id &id);

void sendError (Id *id, ResponseHandler::ErrorCode code, string message, Value *data = nullptr);

void sendError (Id *id, ResponseHandler::ErrorCode code, const char *message, Value *data = nullptr);

void gotoNextHeader ();

size_t readHeaders ();

Document getMessage ();

#endif //LATEX_LANGUAGE_SERVER_MESSAGING_H
