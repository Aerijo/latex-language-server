//
// Created by Benjamin Gray on 18/12/18.
//

#include "communications.h"
#include <mutex>

#define FAILED_LENGTH (-1)

using namespace rapidjson;

using std::string;

int getLength (const string &header) {
    const string length_header = "Content-Length: ";
    const auto length_header_length = length_header.length();

    if (header.length() <= length_header_length) return FAILED_LENGTH;
    if (header.substr(0, length_header_length) != length_header) return FAILED_LENGTH;

    string length_string = header.substr(length_header_length, header.length());

    return std::stoi(length_string);
}

Document awaitAndGetStdinMessage () {
    int exit = 0;
    int length = FAILED_LENGTH;

    string message;
    string header;
    while (true) {
        int c = getchar();

        header += c;

        switch (exit) {
            case 0:
                if (c == '\r') {
                    exit++;
                }
                break;
            case 1:
                if (c == '\n') {
                    exit++;

                    if (length == FAILED_LENGTH) {
                        length = getLength(header);
                    }

                    header = "";
                } else {
                    exit = 0;
                }
                break;
            case 2:
                if (c == '\r') {
                    exit++;
                } else {
                    exit = 0;
                }
                break;
            case 3:
                if (c == '\n') {
                    goto end;
                } else {
                    exit = 0;
                }
                break;
            default:
                exit = 0;
        }
    }

    end:
    for (int i = 0; i < length; i++) {
        message += static_cast<char>(getchar());
    }

    Document document;

    document.Parse(message.c_str(), message.length());

    return document;
};

void sendMessage (Document &json) {
    StringBuffer output;
    PrettyWriter<StringBuffer> writer (output);
    json.Accept(writer);
    sendMessage(output);
}

void sendMessage (StringBuffer &output) {
    std::cout
            << "Content-Length: "
            << output.GetSize()
            << "\r\n\r\n"
            << output.GetString();
    std::cout.flush();
}

void sendMessage (string &output) {
    std::cout
            << "Content-Length: "
            << output.length()
            << "\r\n\r\n"
            << output;
    std::cout.flush();
}