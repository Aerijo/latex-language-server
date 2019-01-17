#ifndef LATEX_LANGUAGE_SERVER_SHUTDOWNHANDLER_H
#define LATEX_LANGUAGE_SERVER_SHUTDOWNHANDLER_H

#include <lsp-tools/Handler.h>

class ShutDownHandler : public RequestHandler {
public:
    ShutDownHandler () : RequestHandler ("shutdown") {};

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_SHUTDOWNHANDLER_H
