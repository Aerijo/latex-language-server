#ifndef LANGUAGESERVER_TOOLS_IOTHREADS_H
#define LANGUAGESERVER_TOOLS_IOTHREADS_H

#include <thread>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

#include "definitions.h"

#include "messaging.h"
#include "QueueManager.h"
#include "Handler.h"

using namespace rapidjson;

void launchStdinLoop ();

void launchStdoutLoop ();

#endif //LANGUAGESERVER_TOOLS_IOTHREADS_H
