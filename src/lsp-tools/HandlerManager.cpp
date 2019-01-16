#include "HandlerManager.h"

HandlerManager *HandlerManager::getInstance() {
    static HandlerManager instance;
    return &instance;
}

optional<RequestHandler *> HandlerManager::getRequestHandler (const char *method) {
    for (RequestHandler *handler : requestHandlers) {
        if (std::strcmp(method, handler->method) == 0) {
            return { handler };
        }
    }

    return {};
}

optional<ResponseHandler *> HandlerManager::getResponseHandler (Id &id) {
    if (id.type == Id::IdType::Number) {
        for (ResponseHandler *handler : numberResponseHandlers) {
            if (handler->id == id) {
                return { handler };
            }
        }
    } else {
        for (ResponseHandler *handler : stringResponseHandlers) {
            if (handler->id == id) {
                return { handler };
            }
        }
    }

    return {};
}

optional<NotificationHandler *> HandlerManager::getNotificationHandler (const char *method) {
    for (NotificationHandler *handler : notificationHandlers) {
        if (std::strcmp(method, handler->method) == 0) {
            return { handler };
        }
    }

    return {};
}

void HandlerManager::registerHandler (RequestHandler *handler) {
    requestHandlers.push_back(handler);
}

void HandlerManager::registerHandler (NotificationHandler *handler) {
    notificationHandlers.push_back(handler);
}
