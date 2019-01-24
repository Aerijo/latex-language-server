#include "ShutDownHandler.h"
#include "messaging.h"

void ShutDownHandler::run (Id &id, optional<Value> &params) {
    INIT_WRITER
    ADD_ID(id);
    ADD_NULL_RESULT();
    SEND_MESSAGE
}
