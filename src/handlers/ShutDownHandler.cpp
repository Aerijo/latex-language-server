#include "ShutDownHandler.h"
#include "lsp-tools/messaging.h"

void ShutDownHandler::run (Id &id, optional<Value> &params) {
    INIT_WRITER
    ADD_ID(id);
    ADD_NULL_RESULT();
    SEND_MESSAGE
}
