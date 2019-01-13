#ifndef LATEX_LANGUAGE_SERVER_HANDLER_H
#define LATEX_LANGUAGE_SERVER_HANDLER_H

#include <vector>
#include <string>
#include <unordered_map>

#include <rapidjson/schema.h>
#include <rapidjson/document.h>
#include <lsp-tools/definitions.h>

using std::string;
using std::optional;

using namespace rapidjson;

/**
 * Identifying messages:
 *
 *
 *                  id | method | params | result | error
 * Request           x      x        ?
 *
 * Response^         x                        ?       ?
 *
 * Notification             x        ?
 *
 *
 * ^ Response apparently can omit both "result" and "error", but a null result is preferred to indicate success
 *
 */

class Handler {
public:
    std::string method;
};

class RequestHandler : Handler {
    virtual void run (Id id, optional<Value> &params) = 0;
};

class NotificationHandler : Handler {
    virtual void run (optional<Value> &params) = 0;
};

class ResponseHandler : Handler {
    virtual void run (optional<Value> &result, optional<Value> &error) = 0;
};

class HandlerManager {
private:
    static HandlerManager *instance;
    std::unordered_map<std::string, Handler*> handlers;

public:
    static HandlerManager *getInstance ();

    void registerHandler (Handler &handler);

    std::optional<Handler*> getHandlerForMethod (const std::string &method);

    void executeHandlerForMessage (Document &message);

    void onMissingMethod (Document &message);

    void onUnhandledMethod (Document &message);
};

#endif //LATEX_LANGUAGE_SERVER_HANDLER_H
