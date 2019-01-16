#include <iostream>

#include <tree_sitter/runtime.h>
#include "config.h"

#include "handlerRegistration.h"
#include "MessageHandler.h"
#include "QueueManager.h"

extern "C" { TSLanguage *tree_sitter_latex(); }

GlobalConfig *g_config;

int main (int argc, char** argv, const char** env) {

    g_config = new GlobalConfig();

//    std::cerr << sizeof(Value) << std::endl;

//    StringBuffer buffer;
//
//    StringWriter writer (buffer);
//
//    Init::ServerCapabilities foo {};
//
//    foo.codeActionProvider = Init::CodeActionOptions {};
//
//    foo.reflect(writer);

    TSParser *parser = ts_parser_new();

    ts_parser_set_language(parser, tree_sitter_latex());

    QueueManager::init();

    std::cerr << "Running latex-language-server...\n";

    auto messageHandler = new MessageHandler();

    messageHandler->init();

    registerHandlers(messageHandler);

    messageHandler->run();

    return 0;
}
