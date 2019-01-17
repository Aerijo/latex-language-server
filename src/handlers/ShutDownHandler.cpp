#include "ShutDownHandler.h"
#include "messaging.h"

void ShutDownHandler::run (Id &id, optional<Value> &params) {
    INIT_WRITER

    writer.Key("id"); id.writeId(writer);

    writer.Key("result"); writer.Null();

    SEND_MESSAGE
}
