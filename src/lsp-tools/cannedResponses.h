#ifndef LATEX_LANGUAGE_SERVER_CANNEDRESPONSES_H
#define LATEX_LANGUAGE_SERVER_CANNEDRESPONSES_H

#include <rapidjson/writer.h>

#include "definitions.h"
#include "cannedResponses.h"
#include "Handler.h"

#define INIT_WRITER StringBuffer buffer; Writer<StringBuffer> writer (buffer);
#define SEND_MESSAGE sendMessage(buffer);

void cancelRequest (Id &id) {
    INIT_WRITER

    writer.StartObject();
        writer.Key("method"); writer.String("$/cancelRequest");
        writer.Key("params"); writer.StartObject();
        writer.Key("id"); id.writeId(writer);
        writer.EndObject();
    writer.EndObject();

    SEND_MESSAGE
}

void sendError (Id *id, ResponseHandler::ErrorCode code, string message, Value *data = nullptr) {
    sendError(id, code, message.c_str(), data);
}

void sendError (Id *id, ResponseHandler::ErrorCode code, const char *message, Value *data = nullptr) {
    INIT_WRITER

    writer.StartObject();
        writer.Key("id");
        if (id == nullptr)
            writer.Null();
        else
            id->writeId(writer);

        writer.Key("error"); writer.StartObject();
            writer.Key("code"); writer.Int64(code);
            writer.Key("message"); writer.String(message);

            if (data != nullptr) {
                writer.Key("data"); data->Accept(writer);
            }

        writer.EndObject();
    writer.EndObject();

    SEND_MESSAGE
}

#endif //LATEX_LANGUAGE_SERVER_CANNEDRESPONSES_H
