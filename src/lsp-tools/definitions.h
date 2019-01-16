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

typedef string CodeActionKind;

enum class TextDocumentSyncKind {
    None = 0,
    Full = 1,
    Incremental = 2
};

enum class CompletionItemKind {
    Text = 1,
    Method = 2,
    Function = 3,
    Constructor = 4,
    Field = 5,
    Variable = 6,
    Class = 7,
    Interface = 8,
    Module = 9,
    Property = 10,
    Unit = 11,
    Value = 12,
    Enum = 13,
    Keyword = 14,
    Snippet = 15,
    Color = 16,
    File = 17,
    Reference = 18,
    Folder = 19,
    EnumMember = 20,
    Constant = 21,
    Struct = 22,
    Event = 23,
    Operator = 24,
    TypeParameter = 25
};

typedef string MarkupKind;

enum class SymbolKind {
    File = 1,
    Module = 2,
    Namespace = 3,
    Package = 4,
    Class = 5,
    Method = 6,
    Property = 7,
    Field = 8,
    Constructor = 9,
    Enum = 10,
    Interface = 11,
    Function = 12,
    Variable = 13,
    Constant = 14,
    String = 15,
    Number = 16,
    Boolean = 17,
    Array = 18,
    Object = 19,
    Key = 20,
    Null = 21,
    EnumMember = 22,
    Struct = 23,
    Event = 24,
    Operator = 25,
    TypeParameter = 26
};

#endif //LATEX_LANGUAGE_SERVER_DEFINITIONS_H
