#include <iostream>

#include "Handler.h"

HandlerManager *HandlerManager::getInstance() {
    static HandlerManager instance;
    return &instance;
}

optional<RequestHandler *> HandlerManager::getRequestHandler (const string &method) {
    auto itr = requestHandlers.find(method);

    if (itr == requestHandlers.end()) {
        return {};
    }

    return { itr->second };
}

optional<ResponseHandler *> HandlerManager::getResponseHandler (const Id &id) {
    if (id.type == IdType::Number) {
        auto itr = numberResponseHandlers.find(id.numberId);

        if (itr == numberResponseHandlers.end()) {
            return {};
        }

        return { itr->second };
    } else {
        auto itr = stringResponseHandlers.find(id.stringId);

        if (itr == stringResponseHandlers.end()) {
            return {};
        }

        return { itr->second };
    }
}

optional<NotificationHandler *> HandlerManager::getNotificationHandler (const string &method) {
    auto itr = notificationHandlers.find(method);

    if (itr == notificationHandlers.end()) {
        return {};
    }

    return { itr->second };
}

