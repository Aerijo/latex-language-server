#include <iostream>

#include <tree_sitter/runtime.h>

#include <QueueManager.h>
#include <ioThreads.h>

extern "C" { TSLanguage *tree_sitter_latex(); }

int main (int argc, char** argv, const char** env) {

    TSParser *parser = ts_parser_new();

    ts_parser_set_language(parser, tree_sitter_latex());

    QueueManager::init();

    std::cerr << "Running latex-language-server...\n";

    launchStdinLoop();

    launchStdoutLoop();

    auto *queue = QueueManager::getInstance();
    while (true) {
        Document message = queue->for_handlers.dequeue();

        if (message.IsNull()) {
            std::cerr << "Message is null :( ...\n";
        }

        // just echo it back
        QueueManager::writeStdout(message);
    }

    return 0;
}
