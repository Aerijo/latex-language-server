#ifndef LATEX_LANGUAGE_SERVER_HOVER_H
#define LATEX_LANGUAGE_SERVER_HOVER_H

#include <lsp-tools/Handler.h>

class HoverHandler : public RequestHandler {
public:
    HoverHandler () : RequestHandler ("textDocument/hover") {};

    void registerCapabilities (Init::ServerCapabilities &capabilities) override;

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_HOVER_H
