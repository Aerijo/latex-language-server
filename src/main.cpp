#include <iostream>

#include <tree_sitter/runtime.h>

#include <QueueManager.h>
#include <ioThreads.h>
#include <lsp-tools/MessageHandler.h>

#include <handlerRegistration.h>

extern "C" { TSLanguage *tree_sitter_latex(); }

int main (int argc, char** argv, const char** env) {


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
