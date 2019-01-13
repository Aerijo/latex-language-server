#include <iostream>

#include "library.h"

#define STDIN_EXPECT(e) if (getchar() != e) { gotoNextHeader(); continue; };

void handleFailedJSON () {

}

/**
 * Check:
 * - If method starts with `$/`, it must be a notification (request throws error MethodNotFound)
 * - Has `jsonrpc: 2.0`
 * - Has `method`
 * -
 */
void verifyJSON (const Document &json) {
    if (json.HasParseError()) return handleFailedJSON();

    Value::ConstMemberIterator version = json.FindMember("jsonrpc");
    if (version == json.MemberEnd() || std::string(version->value.GetString()) != "2.0") {
        handleFailedJSON();
    }
}

void gotoNextHeader () {
    std::cerr << "Failed header...\n";
    while (true) {
        while (getchar() != '\r') {}
        if (getchar() == '\n') return;
    }
}

size_t readHeaders () {
    size_t length = 0;
    string length_str;
    int c;

    while (true) {
        STDIN_EXPECT('C');
        STDIN_EXPECT('o');
        STDIN_EXPECT('n');
        STDIN_EXPECT('t');
        STDIN_EXPECT('e');
        STDIN_EXPECT('n');
        STDIN_EXPECT('t');
        STDIN_EXPECT('-');
        STDIN_EXPECT('L');
        STDIN_EXPECT('e');
        STDIN_EXPECT('n');
        STDIN_EXPECT('g');
        STDIN_EXPECT('t');
        STDIN_EXPECT('h');
        STDIN_EXPECT(':');
        STDIN_EXPECT(' ');
        break;
    }

    int exitDepth;

    while (true) {
        c = getchar();
        if (c == '\r') {
            getchar(); // '\n'
            exitDepth = 2;
            break;
        }
        length_str += static_cast<char>(c);
    }

    length = std::strtoull(length_str.c_str(), nullptr, 10);

    while (true) {
        c = getchar();
        switch (exitDepth) {
            case 0:
            case 2:
                if (c == '\r')
                    exitDepth++;
                else
                    exitDepth = 0;
                break;
            case 1:
                if (c == '\n')
                    exitDepth++;
                else
                    exitDepth = 0;
                break;
            default:
                if (c == '\n')
                    return length;
                exitDepth = 0;
        }
    }
}

// NOTE: Still need to respond
void cancelRequest (int id) {
    StringBuffer buffer;
    Writer<StringBuffer> writer (buffer);

    writer.StartObject();
        writer.Key("method"); writer.String("$/cancelRequest");
        writer.Key("params"); writer.StartObject();
            writer.Key("id"); writer.Int(id);
        writer.EndObject();
    writer.EndObject();

//    sendMessage(buffer);
}

void cancelRequest (const string &id) {
    StringBuffer buffer;
    Writer<StringBuffer> writer (buffer);

    writer.StartObject();
        writer.Key("method"); writer.String("$/cancelRequest");
        writer.Key("params"); writer.StartObject();
            writer.Key("id"); writer.String(id.c_str());
        writer.EndObject();
    writer.EndObject();

//    sendMessage(buffer);
}

/**
 * Extracts and returns the raw JSON from a message. Also verifies the
 * jsonrpc property is valid.
 *
 * @return The JSON message
 */
Document getMessage () {
    size_t length = readHeaders();

    string buffer;

    for (size_t i = 0; i < length; i++) {
        char c = static_cast<char>(getchar());
        std::cerr << c;
        buffer += c;
    }

    Document json;
    json.Parse(buffer.c_str());

    if (json.HasParseError()) {
        std::cerr << "!!!Error parsing JSON\n";
        std::cerr << buffer << "\n";

        auto error = json.GetParseError();
        std::cerr << "Error code: " << error << "\n";
    }

    return json;
}

void awaitInitialization () {

}
