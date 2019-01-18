#ifndef LATEX_LANGUAGE_SERVER_HANDLERMANAGER_H
#define LATEX_LANGUAGE_SERVER_HANDLERMANAGER_H

#include <optional>
#include <vector>
#include <handlers/InitializeHandler.h>

#include "definitions.h"
#include "Handler.h"

using std::vector;

class HandlerManager {
private:
    static HandlerManager *instance;

    vector<RequestHandler *> requestHandlers;
    vector<NotificationHandler *> notificationHandlers;

    // technically don't need to leave them separate, but it's a way to divide them "for free"
    vector<ResponseHandler *> numberResponseHandlers;
    vector<ResponseHandler *> stringResponseHandlers;

public:
    static HandlerManager *getInstance ();

    optional<RequestHandler *> getRequestHandler (const char *method);
    optional<ResponseHandler *> getResponseHandler (Id &id);
    optional<NotificationHandler *> getNotificationHandler (const char *method);

    void registerHandler (RequestHandler *handler);
    void registerHandler (NotificationHandler *handler);

    void registerCapabilities (Init::ServerCapabilities &capabilities);
};

#endif //LATEX_LANGUAGE_SERVER_HANDLERMANAGER_H
