#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include <rapidjson/document.h>

#include "ioThreads.h"


optional<const char *> getString (Document &message, const char *key) {
    auto itr = message.FindMember(key);
    if (itr == message.MemberEnd()) {
        return {};
    }

    return { itr->value.GetString() };
}

void launchStdinLoop () {
    std::cin.tie(nullptr);

    std::thread([=]{
        while (true) {
            Document message = getMessage();

            optional<const char *> method = getString(message, "method");

            if (!method.value()) {
                sendError(nullptr, ResponseHandler::ErrorCode::InvalidRequest, "Was not expecting a response");
                continue;
            }

            if (std::strcmp(method.value(), "initialize") == 0) {
                QueueManager::pushMessage(message);
                break;
            } else if (std::strcmp(method.value(), "exit") == 0) {
                std::cerr << "!!! Received exit notification !!! exiting\n";
                exit(0);
            } else {
                sendError(nullptr, ResponseHandler::ErrorCode::MethodNotFound, "Awaiting initialize request");
            }
        }

        while (true) {
            Document message = getMessage();

            QueueManager::pushMessage(message);
       }
    }).detach();
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
