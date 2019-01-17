#include "CompletionProvider.h"

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

}
