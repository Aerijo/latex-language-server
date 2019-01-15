#ifndef LANGUAGESERVER_TOOLS_IOTHREADS_H
#define LANGUAGESERVER_TOOLS_IOTHREADS_H

#include <thread>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

#include "definitions.h"

using namespace rapidjson;

void cancelRequest (Id &id);

void sendMessage (Document &message);

void sendMessage (StringBuffer &buffer);

void launchStdinLoop ();

void launchStdoutLoop ();

#endif //LANGUAGESERVER_TOOLS_IOTHREADS_H
