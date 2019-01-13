#include <iostream>
#include <thread>
#include <string>

#include <rapidjson/document.h>

#include "QueueManager.h"
#include "library.h"

void launchStdinLoop () {
    std::cin.tie(nullptr);

    std::thread([=]{
        awaitInitialization();

        while (true) {
            Document message = getMessage();

            if (message.IsNull()) {
                std::cerr << "!!!Input message was null...\n";
            }

            QueueManager::pushMessage(message);
       }
    }).detach();
}

void sendMessage (Document &message) {
    if (!message.HasMember("jsonrpc")) {
        message.AddMember("jsonrpc", "2.0", message.GetAllocator());
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer (buffer);
    message.Accept(writer);

    std::cout
            << "Content-Length: " << buffer.GetLength()
            << "\r\n\r\n"
            << buffer.GetString();
    std::cout.flush();
}

void launchStdoutLoop () {
    std::thread([=]{
        auto *queue = QueueManager::getInstance();

        while (true) {
            Document message = queue->for_stdout.dequeue();
            sendMessage(message);
        }
    }).detach();
}
