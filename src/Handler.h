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
using std::unordered_map;

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
    string method;
};

class RequestHandler : Handler {
    virtual void run (Id &id, optional<Value> &params) = 0;
};

class NotificationHandler : Handler {
    virtual void run (optional<Value> &params) = 0;
};

class ResponseHandler : Handler {
    virtual void run (Id &id, optional<Value> &result, optional<Value> &error) = 0;
};

class HandlerManager {
private:
    static HandlerManager *instance;

    unordered_map<string, RequestHandler *> requestHandlers;
    unordered_map<IdNumberType, ResponseHandler *> numberResponseHandlers;
    unordered_map<string, ResponseHandler *> stringResponseHandlers;
    unordered_map<string, NotificationHandler *> notificationHandlers;

public:
    static HandlerManager *getInstance ();

    optional<RequestHandler *> getRequestHandler (const string &method);
    optional<ResponseHandler *> getResponseHandler (const Id &id);
    optional<NotificationHandler *> getNotificationHandler (const string &method);
};

#endif //LATEX_LANGUAGE_SERVER_HANDLER_H
