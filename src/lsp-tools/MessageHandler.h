#ifndef LATEX_LANGUAGE_SERVER_MESSAGEHANDLER_H
#define LATEX_LANGUAGE_SERVER_MESSAGEHANDLER_H

#include <rapidjson/document.h>

using namespace rapidjson;

class MessageHandler {
    void init ();

    void run ();

    void handleRequest (Document &message);

    void handleResponse (Document &message);

    void handleNotification (Document &message);

    void handleUnknown (Document &message);
};

#endif //LATEX_LANGUAGE_SERVER_MESSAGEHANDLER_H
