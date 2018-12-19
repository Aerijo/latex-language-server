//
// Created by Benjamin Gray on 19/12/18.
//

#include "message_handler.h"
#include "communications.h"

#define ASSERT_PROP(json, prop, val) if (!json.HasMember(prop) || std::string(json[prop].GetString()) != val) exit(1);
#define ASSERT_EXISTS(json, prop) if (!json.HasMember(prop)) exit(2);

void assertJsonValid (Document &json) {
    ASSERT_PROP(json, "jsonrpc", "2.0")
}



void handleMessage (Document &json) {
    assertJsonValid(json);


    // Just echo it back right now
    sendMessage(json);
}

void handleInitRequest (Document &json) {
    assertJsonValid(json);

    ASSERT_PROP(json, "method", "initialize");
    ASSERT_EXISTS(json, "id");

    int id = json["id"].GetInt();

    std::string content =
        "{"
            "\"id\":\"" + std::to_string(id) + "\","
            "\"result\":{"
                "\"capabilities\":{"
                    "\"textDocumentSync\":2,"
                    "\"hoverProvider\":true"
                "}"
            "}"
        "}";

    sendMessage(content);
}