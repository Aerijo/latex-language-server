#include <iostream>

#include "messaging.h"

void sendMessage (StringBuffer &buffer) {
    std::cout
            << "Content-Length: " << buffer.GetLength()
            << "\r\n\r\n"
            << buffer.GetString();
    std::cout.flush();
}

void sendMessage (Document &message) {
    if (!message.HasMember("jsonrpc")) {
        message.AddMember("jsonrpc", "2.0", message.GetAllocator());
    }

    StringBuffer buffer;

    Writer<StringBuffer> writer (buffer);
    message.Accept(writer);

    sendMessage(buffer);
}

void cancelRequest (Id &id) {
    INIT_WRITER

    writer.Key("method"); writer.String("$/cancelRequest");
    writer.Key("params"); writer.StartObject();
        writer.Key("id"); id.writeId(writer);
    writer.EndObject();

    SEND_MESSAGE
}

void sendError (Id *id, ResponseHandler::ErrorCode code, const string message, Value *data) {
    const char *c_message = message.c_str();
    sendError(id, code, c_message, data);
}

void sendError (Id *id, ResponseHandler::ErrorCode code, const char *message, Value *data) {
    INIT_WRITER

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

    SEND_MESSAGE
}


#define STDIN_EXPECT(e) if (getchar() != e) { gotoNextHeader(); continue; };

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

Document getMessage () {
    size_t length = readHeaders();

    string buffer;

    for (size_t i = 0; i < length; i++) {
        char c = static_cast<char>(getchar());
        buffer += c;
    }

    std::cerr << buffer << "\n\n";

    Document json;
    json.Parse(buffer);

    if (json.HasParseError()) {
        std::cerr << "!!!Error parsing JSON\n";
        std::cerr << buffer << "\n";

        auto error = json.GetParseError();
        std::cerr << "Error code: " << error << "\n";
    }

    return json;
}
