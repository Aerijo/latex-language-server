//
// Created by Benjamin Gray on 19/12/18.
//

#ifndef LATEX_LANGUAGE_SERVER_MESSAGE_HANDLER_H
#define LATEX_LANGUAGE_SERVER_MESSAGE_HANDLER_H

#include "rapidjson/document.h"

using namespace rapidjson;

void handleMessage (Document &json);

void handleInitRequest (Document &json);

#endif // LATEX_LANGUAGE_SERVER_MESSAGE_HANDLER_H
