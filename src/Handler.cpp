#include <iostream>

#include "Handler.h"

void HandlerManager::registerHandler (Handler &handler) {
    std::string &method = handler.method;

    if (handlers.count(method)) {
        std::cerr << "Multiple handlers attempted for " << method << "; overwriting\n";
    }

    handlers[method] = &handler;
}

std::optional<Handler*> HandlerManager::getHandlerForMethod (const std::string &method) {
    if (handlers.count(method)) {
        return handlers[method];
    }
    return {};
}

void HandlerManager::executeHandlerForMessage (Document &message) {
    auto itr = message.FindMember("method");
    if (itr == message.MemberEnd()) {
        onMissingMethod(message);
        return;
    }

    const std::string method = itr->value.GetString();

    std::optional<Handler*> handler = getHandlerForMethod(method);

    if (handler.has_value()) {
        handler.value()->run(message);
    } else {
        onUnhandledMethod(message);
    }
}

void HandlerManager::onMissingMethod(Document &message) {
    std::cerr << "Missing method for message " << message.GetString() << "\n";
}

void HandlerManager::onUnhandledMethod(Document &message) {
    std::cerr << "Unhandled method for message " << message.GetString() << "\n";
}

HandlerManager *HandlerManager::getInstance() {
    static HandlerManager instance;
    return &instance;
}
