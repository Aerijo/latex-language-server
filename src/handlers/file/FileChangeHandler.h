#ifndef LATEX_LANGUAGE_SERVER_FILECHANGE_H
#define LATEX_LANGUAGE_SERVER_FILECHANGE_H

#include <lsp-tools/Handler.h>

class FileChangeHandler : public NotificationHandler {
public:
    FileChangeHandler () : NotificationHandler ("textDocument/didChange") {};

    void registerCapabilities (Init::ServerCapabilities &capabilities) override;

    void run (optional<Value> &params) override;

    void handleMissingFileChangeParams ();
};

#endif //LATEX_LANGUAGE_SERVER_FILECHANGE_H
