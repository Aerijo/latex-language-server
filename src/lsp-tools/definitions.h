#ifndef LATEX_LANGUAGE_SERVER_DEFINITIONS_H
#define LATEX_LANGUAGE_SERVER_DEFINITIONS_H

#include <string>
#include <rapidjson/writer.h>

#define IdNumberType int_fast64_t

using std::string;

using namespace::rapidjson;

enum class IdType {
    Number,
    String
};

class Id {
    IdType type;
    union {
        string stringId;
        IdNumberType numberId;
    };

    void writeId (Writer<StringBuffer> &writer) {
        switch (type) {
            case IdType::Number:
                writer.Int64(numberId);
                break;
            case IdType::String:
                writer.String(stringId.c_str());
                break;
        }
    }

    bool operator == (Id &that) {
        return type != that.type
            ? false
            : type == IdType::Number
                ? numberId == that.numberId
                : stringId == that.stringId;
    }
};

enum class MessageType {
    Request,
    Response,
    Notification,
    Unknown
};

#endif //LATEX_LANGUAGE_SERVER_DEFINITIONS_H
