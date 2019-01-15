#include "initialize.h"

#include <iostream>
#include <lsp-tools/ioThreads.h>

#include "lsp-tools/Handler.h"

void InitializeHandler::run (Id &id, optional<Value> &params) {
    std::cerr << "handling initialisation request...\n";

    StringBuffer buffer;

    Writer<StringBuffer> writer (buffer);

    writer.StartObject();
        writer.
    writer.EndObject();

    sendMessage(buffer);
}
