#ifndef LATEX_LANGUAGE_SERVER_HANDLER_H
#define LATEX_LANGUAGE_SERVER_HANDLER_H

#include <optional>

#include <rapidjson/document.h>

#include "definitions.h"

using std::optional;

using rapidjson::Value;

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
class Handler {
public:
    enum class Type {
        Request,
        Response,
        Notification
    };

    virtual void registerCapabilities (Init::ServerCapabilities &capabilities) {};

protected:
    explicit Handler (Handler::Type type) : type (type) {}

    Handler::Type type;

};

class RequestHandler : public Handler {
public:
    explicit RequestHandler (const char *method) : Handler (Handler::Type::Request), method (method) {}

    const char *method;

    virtual void run (Id &id, optional<Value> &params) = 0;
};

class NotificationHandler : public Handler {
public:
    explicit NotificationHandler (const char *method) : Handler (Handler::Type::Notification), method (method) {}

    const char *method;

    virtual void run (optional<Value> &params) = 0;
};

class ResponseHandler : public Handler {
public:
    explicit ResponseHandler (Id id) : Handler (Handler::Type::Response), id (id) {}

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


#endif //LATEX_LANGUAGE_SERVER_HANDLER_H
