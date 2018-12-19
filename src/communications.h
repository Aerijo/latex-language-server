//
// Created by Benjamin Gray on 18/12/18.
//

#ifndef LATEX_LANGUAGE_SERVER_COMMUNICATIONS_H
#define LATEX_LANGUAGE_SERVER_COMMUNICATIONS_H

#include <optional>
#include <iostream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "rapidjson/document.h"

rapidjson::Document awaitAndGetStdinMessage ();

void sendMessage (rapidjson::Document &json);

void sendMessage (rapidjson::StringBuffer &output);

void sendMessage (std::string &output);

#endif // LATEX_LANGUAGE_SERVER_COMMUNICATIONS_H