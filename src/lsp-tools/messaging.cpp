#include <iostream>
#include <thread>

#include "messaging.h"
#include "QueueManager.h"

void sendMessage (StringBuffer &buffer) {
    std::cout
            << "Content-Length: " << buffer.GetLength()
            << "\r\n\r\n"
            << buffer.GetString();
    std::cout.flush();

//    std::cerr << "OUTGOING\n";
//    std::cerr
//            << buffer.GetString();
//    std::cerr.flush();
}

void sendMessage (Document &message) {
    StringBuffer buffer;

    StringWriter writer (buffer);
    message.Accept(writer);

    sendMessage(buffer);
}

void cancelRequest (Id &id) {
    INIT_WRITER

    ADD_METHOD("$/cancelRequest");
    writer.Key("params"); writer.StartObject();
    ADD_ID(id);
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

void sendNullResult (Id &id) {
    INIT_WRITER
    ADD_ID(id);
    ADD_NULL_RESULT();
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

//    std::cerr << "INCOMING\n";
//    std::cerr << buffer << "\n\n";

    Document json;
    json.Parse(buffer);

    if (json.HasParseError()) {
        std::cerr << "!!!Error parsing JSON\n";
        std::cerr << buffer << "\n";

        auto error = json.GetParseError();
        std::cerr << "Error code: " << error << "\n";

        sendError(nullptr, ResponseHandler::ErrorCode::ParseError, "Failed to parse JSON");

        return getMessage();
    }

    return json;
}

optional<const char *> getString (Document &message, const char *key) {
    auto itr = message.FindMember(key);
    if (itr == message.MemberEnd()) {
        return {};
    }

    return optional { itr->value.GetString() };
}

void launchStdinLoop () {
    std::cin.tie(nullptr);

    std::thread([=]{
        while (true) {
            Document message = getMessage();

            optional<const char *> method = getString(message, "method");

            if (!method) {
                sendError(nullptr, ResponseHandler::ErrorCode::InvalidRequest, "Was not expecting a response");
                continue;
            }

            if (std::strcmp(*method, "initialize") == 0) {
                QueueManager::pushMessage(message);
                break;
            } else if (std::strcmp(*method, "exit") == 0) {
                std::cerr << "!!! Received exit notification !!! exiting\n";
                exit(0);
            } else {
                sendError(nullptr, ResponseHandler::ErrorCode::MethodNotFound, "Awaiting initialize request");
            }
        }

        while (true) {
            Document message = getMessage();

            if (message.HasMember("method")) {
                const char *textDocument = "textDocument/";
                const size_t len = strlen(textDocument);
                const char *method = message["method"].GetString();

                if (std::strcmp(method, "exit") == 0) {
                    std::cerr << "!!! Received exit notification !!! exiting\n";
                    exit(0);
                }

                if (strncmp(textDocument, method, len) == 0) {
                    if (strcmp("didOpen", method + len) == 0 || strcmp("didChange", method + len) == 0) {
                        QueueManager::pushMessage(message, true);
                        continue;
                    }
                }
            }

            QueueManager::pushMessage(message);
        }
    }).detach();
}

void launchStdoutLoop () {
    std::thread([=]{
        auto *queue = QueueManager::getInstance();

        while (true) {
            Document message = queue->for_stdout.dequeue();
            sendMessage(message);
        }

    }).detach();
}
