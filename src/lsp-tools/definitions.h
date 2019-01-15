#ifndef LATEX_LANGUAGE_SERVER_DEFINITIONS_H
#define LATEX_LANGUAGE_SERVER_DEFINITIONS_H

#include <string>
#include <rapidjson/writer.h>

#define IdNumberType int_fast64_t

using std::string;

using namespace::rapidjson;

struct Id {
    Id () = delete; // disallow "blank" Id objects; if it exists, it must be tied to a message

    explicit Id (IdNumberType val) : type (IdType::Number), numberId (val) {}

    explicit Id (const char *val) : type (IdType::String), stringId (val) {};

    enum class IdType { String, Number } type;

    union {
        IdNumberType numberId;
        const char *stringId;
    };

    void writeId (Writer<StringBuffer> &writer) {
        switch (type) {
            case IdType::Number:
                writer.Int64(numberId);
                break;
            case IdType::String:
                writer.String(stringId);
                break;
        }
    }

    bool operator == (Id &that) {
        return type != that.type
            ? false
            : type == IdType::Number
                ? numberId == that.numberId
                : *stringId == *that.stringId;
    }
};

enum class MessageType {
    Request,
    Response,
    Notification,
    Unknown
};

#endif //LATEX_LANGUAGE_SERVER_DEFINITIONS_H
