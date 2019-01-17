#include <iostream>

#include <tree_sitter/runtime.h>
#include <filesystem/Uri.h>

#include "filesystem/FileManager.h"
#include "config.h"

#include "handlerRegistration.h"
#include "MessageHandler.h"
#include "QueueManager.h"

extern "C" { TSLanguage *tree_sitter_latex(); }

GlobalConfig *g_config;

int main (int argc, char** argv, const char** env) {
    std::cerr << "Running latex-language-server...\n";

    g_config = new GlobalConfig();

    TSParser *parser = ts_parser_new();

    ts_parser_set_language(parser, tree_sitter_latex());

    FileManager::init();
    QueueManager::init();

    auto messageHandler = new MessageHandler();

    messageHandler->init();

    registerHandlers(messageHandler);

    messageHandler->run();

    return 0;
}
