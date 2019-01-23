#include <iostream>
#include <optional>
#include <string>

#include "definitions.h"

#include "Handler.h"
#include "ioThreads.h"
#include "MessageHandler.h"
#include "messaging.h"
#include "QueueManager.h"

//using std::optional;
using std::string;

using rapidjson::Value;

void MessageHandler::init () {
    handlers = HandlerManager::getInstance();

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

void MessageHandler::run () {
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

optional<Value> getObject (Document &value, const string &key) {
    auto itr = value.FindMember(key.c_str());
    if (itr == value.MemberEnd() || itr->value.IsNull()) {
        return {};
    }
    return { itr->value.GetObject() };
}

optional<string> getString (Document &value, const string &key) {
    auto itr = value.FindMember(key.c_str());
    if (itr == value.MemberEnd() || itr->value.IsNull()) {
        return {};
    }
    return { itr->value.GetString() };
}

Id getId (Document &value) {
    auto itr = value.FindMember("id");

    // we assume it exists
    if (itr->value.IsNumber()) {
        return Id { itr->value.GetInt64() };
    } else {
        return Id { itr->value.GetString() };
    }
}

void MessageHandler::handleRequest (Document &message) {
    const char *method = message["method"].GetString();

    Id id = getId(message);

    optional<Value> params = getObject(message, "params");

    optional<RequestHandler *> handler = handlers->getRequestHandler(method);

    if (handler) {
        (*handler)->run(id, params);
    } else {
        std::cerr << "Unhandled request " << method << "\n";

        sendError(&id, ResponseHandler::ErrorCode::MethodNotFound, "No handler for request: " + string(method));
    }
}

void MessageHandler::handleResponse (Document &message) {

}

void MessageHandler::handleNotification (Document &message) {
    const char *method = message["method"].GetString();

    optional<Value> params = getObject(message, "params");

    optional<NotificationHandler *> handler = handlers->getNotificationHandler(method);

    if (handler) {
        (*handler)->run(params);
    } else {
//        Not handling a notification is hardly exceptional
//        std::cerr << "Unhandled notification " << method << "\n";
    }
}

void MessageHandler::handleUnknown (Document &message) {

}


void MessageHandler::registerHandler (RequestHandler *handler) {
    handlers->registerHandler(handler);
}

void MessageHandler::registerHandler (NotificationHandler *handler) {
    handlers->registerHandler(handler);
}
