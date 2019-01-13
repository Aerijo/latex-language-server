#include "MessageHandler.h"
#include "ioThreads.h"
#include "QueueManager.h"
#include "definitions.h"

void MessageHandler::init() {
    launchStdinLoop();

    launchStdoutLoop();
}

bool verifyJsonRpc (Document &json) {
    if (json.HasParseError()) return false;

    auto itr = json.FindMember("jsonrpc");

    if (itr == json.MemberEnd()) return false;

    return itr->value.IsString() && string(itr->value.GetString()) == "2.0";
}

MessageType getMessageType (Document &message) {
    bool hasId = message.HasMember("id");
    bool hasMethod = message.HasMember("method");

    if (hasId) {
        if (hasMethod) {
            return MessageType::Request;
        }
        return MessageType::Response;
    }

    if (hasMethod) {
        return MessageType::Notification;
    }

    return MessageType::Unknown;
}

void MessageHandler::run() {
    auto *queue = QueueManager::getInstance();
    while (true) {
        Document message = queue->for_handlers.dequeue();

        if (!verifyJsonRpc(message)) {
            std::cerr << "Error in JSON RPC parsing; skipping message\n";
            continue;
        }

        switch (getMessageType(message)) {
            case MessageType::Request:
                handleRequest(message);
                break;
            case MessageType::Response:
                handleResponse(message);
                break;
            case MessageType::Notification:
                handleNotification(message);
                break;
            case MessageType::Unknown:
                handleUnknown(message);
                break;
        }
    }
}
