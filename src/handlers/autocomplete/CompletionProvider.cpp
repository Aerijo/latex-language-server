#include "CompletionProvider.h"

#include "messaging.h"

void CompletionProvider::registerCapabilities (Init::ServerCapabilities &capabilities) {
    if (!capabilities.completionProvider) capabilities.completionProvider = Init::CompletionOptions {};

    /*
     * \ - command
     * @ - bib item
     * ! - magic comment
     *
     */
    capabilities.completionProvider->triggerCharacters = { "\\", "@", "!" };
}

void CompletionProvider::run (Id &id, optional<Value> &params) {
    INIT_WRITER
    ADD_ID(id);
    ADD_NULL_RESULT();
    SEND_MESSAGE
}
