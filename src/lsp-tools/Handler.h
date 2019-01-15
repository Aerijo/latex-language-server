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
using std::vector;

using namespace rapidjson;

/**
 * Identifying messages:
 *
 *
 *                  id | method | params | result | error | jsonrpc
 * Request           x      x        ?                         x
 *
 * Response^         x                        ?       ?        x
 *
 * Notification             x        ?                         x
 *
 *
 * ^ Response apparently can omit both "result" and "error", but a null result is preferred to indicate success
 *
 */

enum class HandlerType {
    Request,
    Response,
    Notification
};

class Handler {
protected:
    explicit Handler (HandlerType type) : type (type) {}

    HandlerType type;
};

class RequestHandler : public Handler {
public:
    explicit RequestHandler (const char *method) : Handler (HandlerType::Request), method (method) {}

    const char *method;

    virtual void run (Id &id, optional<Value> &params) = 0;
};

class NotificationHandler : public Handler {
public:
    explicit NotificationHandler (const char *method) : Handler (HandlerType::Notification), method (method) {}

    const char *method;

    virtual void run (optional<Value> &params) = 0;
};

class ResponseHandler : public Handler {
public:
    explicit ResponseHandler (Id id) : Handler (HandlerType::Response), id (id) {}

    Id id;

    virtual void run (Id &id, optional<Value> &result, optional<Value> &error) = 0;

    enum ErrorCode {
        ParseError = -32700,
        InvalidRequest = -32600,
        MethodNotFound = -32601,
        InvalidParams = -32602,
        InternalError = -32603,
        serverErrorStart = -32099,
        serverErrorEnd = -32000,
        ServerNotInitialized = -32002,
        UnknownErrorCode = -32001,
        RequestCancelled = -32800,
        ContentModified = -32801
    };
};

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
};

#endif //LATEX_LANGUAGE_SERVER_HANDLER_H
