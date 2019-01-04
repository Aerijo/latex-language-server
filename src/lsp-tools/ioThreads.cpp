#include <iostream>
#include <thread>
#include <string>

#include <rapidjson/document.h>

#include "QueueManager.h"
#include "library.h"

bool isExitNotif (const Document &message) {
    return std::strcmp(message["method"].GetString(), "exit") == 0;
}

void launchStdinLoop () {
    std::cin.tie(nullptr);

    std::cerr << "launching input loop...\n";

    std::thread([=]{
        std::cerr << "running input loop...\n";
        while (true) {
            std::cerr << "Getting message...\n";
            Document message = getMessage();

            if (message.IsNull()) {
                std::cerr << "!!!Input message was null...\n";
            } else {
                std::cerr << "***Input message was fine...\n";
            }

            QueueManager::pushMessage(message);
       }
    }).detach();
}

void sendMessage (Document &message) {
    std::cerr << "sending message...\n";

//    if (!message.HasMember("jsonrpc")) {
//        message.AddMember("jsonrpc", "2.0", message.GetAllocator());
//    }

    std::cerr << "added jsonrpc...\n";

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
    std::cerr << "launching output loop...\n";
    std::thread([=]{
        auto *queue = QueueManager::getInstance();
        std::cerr << "running output loop...\n";

        while (true) {
            Document message = queue->for_stdout.dequeue();
            sendMessage(message);
        }
    }).detach();
}