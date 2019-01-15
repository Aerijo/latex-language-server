#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include <rapidjson/document.h>

#include "cannedResponses.h"
#include "ioThreads.h"
#include "QueueManager.h"

void sendMessage (Document &message) {
    if (!message.HasMember("jsonrpc")) {
        message.AddMember("jsonrpc", "2.0", message.GetAllocator());
    }

    StringBuffer buffer;

    Writer<StringBuffer> writer (buffer);
    message.Accept(writer);

    sendMessage(buffer);
}

void sendMessage (StringBuffer &buffer) {
    std::cout
            << "Content-Length: " << buffer.GetLength()
            << "\r\n\r\n"
            << buffer.GetString();
    std::cout.flush();
}

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
