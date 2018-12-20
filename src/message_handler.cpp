//
// Created by Benjamin Gray on 19/12/18.
//

#include <unordered_map>
#include <functional>

#include "message_handler.h"
#include "communications.h"

#define ASSERT_PROP(json, prop, val) if (!json.HasMember(prop) || std::string(json[prop].GetString()) != val) exit(1);
#define ASSERT_EXISTS(json, prop) if (!json.HasMember(prop)) exit(2);

std::string getId (Document &message) {
    ASSERT_EXISTS(message, "id");

    if (message["id"].IsInt()) {
        int id = message["id"].GetInt();
        return std::to_string(id);
    } else {
        return std::string(message["id"].GetString());
    }
}

void assertJsonValid (Document &json) {
    ASSERT_PROP(json, "jsonrpc", "2.0")
}

void requestShowMessage (std::string &message) {
    std::string json =
            "{"
                "\"jsonprc\":\"2.0\","
                "\"method\":\"window/showMessage\","
                "\"params\":{"
                    "\"type\":3,"
                    "\"message\":\"" + message + "\""
                "}"
            "}";

    sendMessage(json);
}

void performShutdownSequence () {
    std::string message = "Shutting down LaTeX language server";
    requestShowMessage(message);
}

void sendShutdownResponse (std::string &id) {
    StringBuffer buffer;
    Writer<StringBuffer> writer (buffer);

    writer.StartObject();
    writer.Key("id");
    writer.String(id.c_str());
    writer.Key("result");
    writer.Null();
    writer.EndObject();

    sendMessage(buffer);
}

void handleShutdown (Document &message) {
    performShutdownSequence();

    std::string id = getId(message);
    sendShutdownResponse(id);
}

void handleExit (Document &message) {
    exit(0);
}

void handleTextDocumentChange (Document &message) {

}

static const std::unordered_map<std::string, std::function<void(Document &json)>> handlers {
        {"shutdown", handleShutdown},
        {"exit", handleExit},
        {"textDocument/didChange", handleTextDocumentChange}
};

void genericMessageHandler (Document &message) {
    sendMessage(message);
}



void handleMessage (Document &message) {
    assertJsonValid(message);

    std::string method = message["method"].GetString();

    auto handler = handlers.find(method);

    if (handler == handlers.end()) {
        // Just echo it back if we don't handle it
        return genericMessageHandler(message);
    }

    return handler->second(message);

}

void handleInitRequest (Document &json) {
    assertJsonValid(json);

    ASSERT_PROP(json, "method", "initialize");
    ASSERT_EXISTS(json, "id");

    std::string id = getId(json);

    std::string content =
        "{"
            "\"id\":\"" + id + "\","
            "\"result\":{"
                "\"capabilities\":{"
                    "\"textDocumentSync\":2,"
                    "\"completionProvider\":{"
                        "\"triggerCharacters\":[\"\\\\\"]"
                    "}"
                "}"
            "}"
        "}";

    sendMessage(content);
}