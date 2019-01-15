#ifndef LATEX_LANGUAGE_SERVER_CONFIG_H
#define LATEX_LANGUAGE_SERVER_CONFIG_H

#include <optional>
#include <string>
#include <vector>

#include <rapidjson/document.h>

using std::optional;
using std::string;
using std::vector;

using namespace rapidjson;

struct DocumentUri {
    DocumentUri() = delete;

    explicit DocumentUri (const char *uri) : uri (uri) {}

    const char *uri;


};

struct ClientCapabilities {

};

struct WorkspaceFolder {
    string uri;

    string name;
};

struct LspConfig {
    optional<pid_t> processId {};

    // @deprecated rootPath; use rootUri

    optional<DocumentUri> rootUri {};

    optional<Value *> initializationOptions {};

    ClientCapabilities capabilities {};

    string trace { "off" };

    optional<vector<WorkspaceFolder>> workspaceFolders;
};

struct LatexConfig {

};

struct Config {
    LspConfig lsp {};
    LatexConfig latex {};
};

extern Config *g_config;

#endif //LATEX_LANGUAGE_SERVER_CONFIG_H
