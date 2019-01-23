#ifndef LATEX_LANGUAGE_SERVER_FILEOPENHANDLER_H
#define LATEX_LANGUAGE_SERVER_FILEOPENHANDLER_H

#include <lsp-tools/Handler.h>

class FileOpenHandler : public NotificationHandler {
public:
    FileOpenHandler () : NotificationHandler ("textDocument/didOpen") {};

    void run (optional<Value> &params) override;

    void handleMissingFileOpenParams ();
};

#endif //LATEX_LANGUAGE_SERVER_FILEOPENHANDLER_H
