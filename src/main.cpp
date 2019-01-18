#include <iostream>

#include <tree_sitter/runtime.h>
#include <filesystem/Uri.h>

#include "filesystem/FileManager.h"
#include "lconfig.h"

#include "handlerRegistration.h"
#include "MessageHandler.h"
#include "QueueManager.h"

#include "text-buffer.h"

extern "C" { TSLanguage *tree_sitter_biber(); }

GlobalConfig *g_config;

int main (int argc, char** argv, const char** env) {
    std::cerr << "Running latex-language-server...\n";

    Uri foo = Uri::parse("file:///home/benjamin/test/exp.tex");

    foo.print();

    g_config = new GlobalConfig();

    TextBuffer buffer {};

//    buffer.find_words_with_subsequence_in_range({ "foo" }, "bar", { { 0,0}, {3,4}});

//    Text bazx = buffer

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
