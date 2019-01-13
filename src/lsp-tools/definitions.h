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

struct Id {
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
};

#endif //LATEX_LANGUAGE_SERVER_DEFINITIONS_H
