//
// Created by Benjamin Gray on 19/12/18.
//

#include "message_handler.h"
#include "communications.h"

#define ASSERT_PROP(json, prop, val) if (!json.HasMember(prop) || std::string(json[prop].GetString()) != val) exit(1);

void assertJsonValid (Document &json) {
    ASSERT_PROP(json, "jsonrpc", "2.0")
}



void handleMessage (Document &json) {
    assertJsonValid(json);
}

void handleInitRequest (Document &json) {
    assertJsonValid(json);

    ASSERT_PROP(json, "method", "initialize");

    std::string content =
        "{"
            "\"id\":1,"
            "\"result\":{"
                "\"capabilities\":{"
                    "\"textDocumentSync\":2"
                "}"
            "}"
        "}";

    sendMessage(content);
}