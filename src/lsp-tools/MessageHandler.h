#ifndef LATEX_LANGUAGE_SERVER_MESSAGEHANDLER_H
#define LATEX_LANGUAGE_SERVER_MESSAGEHANDLER_H

#include <rapidjson/document.h>
#include <Handler.h>

using namespace rapidjson;

class MessageHandler {
private:
    HandlerManager *handlers;

public:
    void init ();

    void run ();

    void handleRequest (Document &message);

    void handleResponse (Document &message);

    void handleNotification (Document &message);

    void handleUnknown (Document &message);

    void registerHandler (RequestHandler *handler);

    void registerHandler (NotificationHandler *handler);
};

#endif //LATEX_LANGUAGE_SERVER_MESSAGEHANDLER_H
