#include <iostream>
#include <string>

#include <locale>

#include <tree_sitter/runtime.h>
#include <filesystem/Uri.h>

#include "filesystem/FileManager.h"
#include "lconfig.h"

#include "handlerRegistration.h"
#include "MessageHandler.h"
#include "QueueManager.h"

#include "filesystem/UtfHandler.h"

#include "text-buffer.h"

extern "C" { TSLanguage *tree_sitter_biber(); }

using std::u16string;

GlobalConfig *g_config;

int main (int argc, char** argv, const char** env) {
    std::cerr << "Running latex-language-server...\n";

    g_config = new GlobalConfig();

    UtfHandler utf {};

    u16string bar = u"abc" "\xD800" "def";

    string foo = utf.utf16to8(bar);

    std::cout << foo << std::endl;

    TSParser *parser = ts_parser_new();

    ts_parser_set_language(parser, tree_sitter_biber());

    FileManager::init();
    QueueManager::init();

    auto messageHandler = new MessageHandler();

    messageHandler->init();

    registerHandlers(messageHandler);

    messageHandler->run();

    return 0;
}
