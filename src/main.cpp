#include <iostream>

#include <tree_sitter/runtime.h>

#include <QueueManager.h>
#include <ioThreads.h>

int main (int argc, char** argv, const char** env) {

    TSParser *parser = ts_parser_new();


    QueueManager::init();

    std::cerr << "Running latex-language-server...\n";

    launchStdinLoop();

    std::cerr << "set up input loop...\n";

    launchStdoutLoop();

    std::cerr << "Running main loop...\n";

    auto *queue = QueueManager::getInstance();
    while (true) {
        Document message = queue->for_handlers.dequeue();

        std::cerr << "Handling message...\n";

        if (message.IsNull()) {
            std::cerr << "Message is null :( ...\n";
        }


        QueueManager::writeStdout(message);
    }

    return 0;
}
