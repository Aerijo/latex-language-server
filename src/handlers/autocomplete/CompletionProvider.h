#ifndef LATEX_LANGUAGE_SERVER_COMPLETIONPROVIDER_H
#define LATEX_LANGUAGE_SERVER_COMPLETIONPROVIDER_H

#include <lsp-tools/Handler.h>

class CompletionProvider : public RequestHandler {
public:
    CompletionProvider () : RequestHandler ("textDocument/completion") {};

    void registerCapabilities (Init::ServerCapabilities &capabilities) override;

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_COMPLETIONPROVIDER_H
