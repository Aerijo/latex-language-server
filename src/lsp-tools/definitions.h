#ifndef LATEX_LANGUAGE_SERVER_DEFINITIONS_H
#define LATEX_LANGUAGE_SERVER_DEFINITIONS_H

#include <string>
#include <vector>

#include <optional.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

using std::string;
using std::vector;

using rapidjson::Value;
using rapidjson::Writer;
using rapidjson::StringBuffer;

typedef int_fast64_t IdNumberType;
typedef int_fast64_t versionNum;

/*
 * The following macros are used for convenience when making
 * JSON strings. They expect the name `writer` for a StringWriter
 * variable.
 *
 * We use macros over inline functions because I don't like passing
 * `writer` as an argument each time
 */

#define INIT_WRITER \
    StringBuffer buffer;\
    Writer<StringBuffer> writer (buffer); \
    writer.StartObject(); \
    writer.Key("jsonrpc"); \
    writer.String("2.0");

#define SEND_MESSAGE \
    writer.EndObject(); \
    sendMessage(buffer);

#define ADD_METHOD(method) \
    writer.Key("method"); \
    writer.String(method)

#define ADD_ID(id) \
    writer.Key("id"); \
    id.writeId(writer)

#define ADD_TS_POINT(point) \
    writer.StartObject(); \
    writer.Key("line"); \
    writer.Uint64(point.row); \
    writer.Key("character"); \
    writer.Uint64(point.column >> 1); /* shift 1 because a TSPoint measures by byte, and LSP uses UTF16 width for character */ \
    writer.EndObject()

#define ADD_TS_RANGE(range) \
    writer.StartObject(); \
    writer.Key("start"); \
    ADD_TS_POINT(range.start_point); \
    writer.Key("end"); \
    ADD_TS_POINT(range.end_point); \
    writer.EndObject()

#define ADD_NULL_RESULT() \
    writer.Key("result"); \
    writer.Null()

typedef Writer<StringBuffer> StringWriter;

struct Id {
    Id () = delete; // disallow "blank" Id objects; if it exists, it must be tied to a message

    explicit Id (IdNumberType val) : type (IdType::Number), numberId (val) {}

    explicit Id (const char *val) : type (IdType::String), stringId (val) {};

    enum class IdType { String, Number } type;

    union {
        IdNumberType numberId;
        const char *stringId;
    };

    void writeId (StringWriter &writer) {
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

namespace Init {

    struct Reflectable {
        virtual void reflect (StringWriter &writer) = 0;

//        virtual ~Reflectable () = default;
    };

    template<typename T>
    void reflectVector (StringWriter &writer, const string &name, vector<T> &list, bool skipIfEmpty = true) {
        if (skipIfEmpty && list.empty()) return;

        writer.Key(name);
        writer.StartArray();
        for (T &elem : list) {
            elem.reflect(writer);
        }
        writer.EndArray();
    };

    template<>
    void reflectVector<string> (StringWriter &writer, const string &name, vector<string> &list, bool skipIfEmpty);

    void reflectBool (StringWriter &writer, const string &name, bool value, bool skipIfFalse = true);

    void reflectInt (StringWriter &writer, const string &name, int value, bool skipIfNegative = true);

    void reflectObject (StringWriter &writer, const string &name, Reflectable &value);

    void reflectString (StringWriter &writer, const string &name, string &value, bool skipIfEmpty = true);

    template <typename T>
    void reflectOptionalObject (StringWriter &writer, const string &name, optional<T> &value) {
        if (value) {
            reflectObject(writer, name, *value);
        }
    }

    struct SaveOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectBool(writer, "includeText", includeText);
            writer.EndObject();
        }

        bool includeText { false };
    };

    struct TextDocumentSyncOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectBool(writer, "openClose", openClose);
            reflectInt(writer, "change", (int) change);
            reflectBool(writer, "willSave", willSave);
            reflectBool(writer, "willSaveWaitUntil", willSaveWaitUntil);
            reflectOptionalObject(writer, "save", save);
            writer.EndObject();
        }

        bool openClose { false };

        TextDocumentSyncKind change { TextDocumentSyncKind::None };

        bool willSave { false };

        bool willSaveWaitUntil { false };

        optional<SaveOptions> save {};
    };

    struct CompletionOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectBool(writer, "resolveProvider", resolveProvider);
            reflectVector(writer, "triggerCharacters", triggerCharacters);
            writer.EndObject();
        }

        bool resolveProvider { false };

        vector<string> triggerCharacters {};
    };

    struct SignatureHelpOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectVector(writer, "triggerCharacters", triggerCharacters);
            writer.EndObject();
        }

        vector<string> triggerCharacters {};
    };

    struct DocumentFilter : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectString(writer, "language", language);
            reflectString(writer, "scheme", scheme);
            reflectString(writer, "pattern", pattern);
            writer.EndObject();
        }

        string language {};

        string scheme {};

        string pattern {};
    };

    struct TextDocumentRegistrationOptions {
        vector<DocumentFilter> documentSelector {};
    };

    struct StaticRegistrationOptions {
        string id {};
    };

    struct RegistrationOptions : StaticRegistrationOptions, TextDocumentRegistrationOptions, Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectVector<DocumentFilter>(writer, "documentSelector", documentSelector);
            reflectString(writer, "id", id);
            writer.EndObject();
        }
    };

    struct CodeActionOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectVector(writer, "codeActionKinds", codeActionKinds);
            writer.EndObject();
        }

        vector<CodeActionKind> codeActionKinds {};
    };

    struct CodeLensOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectBool(writer, "resolveProvider", resolveProvider);
            writer.EndObject();
        }

        bool resolveProvider { false };
    };

    struct DocumentOnTypeFormattingOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectString(writer, "firstTriggerCharacter", firstTriggerCharacter);
            reflectVector(writer, "moreTriggerCharacter", moreTriggerCharacter);
            writer.EndObject();
        }

        string firstTriggerCharacter {};

        vector<string> moreTriggerCharacter {};
    };

    struct RenameOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectBool(writer, "prepareProvider", prepareProvider);
            writer.EndObject();
        }

        bool prepareProvider { false };
    };

    struct DocumentLinkOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectBool(writer, "resolveProvider", resolveProvider);
            writer.EndObject();
        }

        bool resolveProvider { false };
    };

    struct ColorProviderOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            writer.EndObject();
        }
    };

    struct FoldingRangeProviderOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            writer.EndObject();
        }
    };

    struct ExecuteCommandOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectVector(writer, "commands", commands);
            writer.EndObject();
        }

        vector<string> commands {};
    };

    struct WorkspaceFolders : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectBool(writer, "supported", supported);
            reflectString(writer, "changeNotifications", changeNotifications);
            writer.EndObject();
        }

        bool supported { false };

        string changeNotifications {};
    };

    struct Workspace : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectOptionalObject(writer, "workSpaceFolders", workspaceFolders);
            writer.EndObject();
        }

        optional<WorkspaceFolders> workspaceFolders {};
    };

    struct ServerCapabilities : Reflectable {
        void reflect (StringWriter &writer) final {
            writer.StartObject();
            reflectOptionalObject(writer, "textDocumentSync", textDocumentSync);
            reflectBool(writer, "hoverProvider", hoverProvider);
            reflectOptionalObject(writer, "completionProvider", completionProvider);
            reflectOptionalObject(writer, "signatureHelpProvider", signatureHelpProvider);
            reflectBool(writer, "definitionProvider", definitionProvider);
            reflectOptionalObject(writer, "typeDefinitionProvider", typeDefinitionProvider);
            reflectOptionalObject(writer, "implementationProvider", implementationProvider);
            reflectBool(writer, "referencesProvider", referencesProvider);
            reflectBool(writer, "documentHighlightProvider", documentHighlightProvider);
            reflectBool(writer, "documentSymbolProvider", documentSymbolProvider);
            reflectBool(writer, "workplaceSymbolProvider", workplaceSymbolProvider);
            reflectOptionalObject(writer, "codeActionProvider", codeActionProvider);
            reflectOptionalObject(writer, "codeLensProvider", codeLensProvider);
            reflectBool(writer, "documentFormattingProvider", documentFormattingProvider);
            reflectOptionalObject(writer, "documentOnTypeFormattingProvider", documentOnTypeFormattingProvider);
            reflectOptionalObject(writer, "renameProvider", renameProvider);
            reflectOptionalObject(writer, "documentLinkProvider", documentLinkProvider);
            reflectOptionalObject(writer, "colorProvider", colorProvider);
            reflectOptionalObject(writer, "foldingRangeProvider", foldingRangeProvider);
            reflectOptionalObject(writer, "executeCommandProvider", executeCommandProvider);
            reflectOptionalObject(writer, "workspace", workspace);
            // TODO: Reflect experimental Value ?
            writer.EndObject();
        }

        optional<TextDocumentSyncOptions> textDocumentSync {};

        bool hoverProvider { false };

        optional<CompletionOptions> completionProvider {};

        optional<SignatureHelpOptions> signatureHelpProvider {};

        bool definitionProvider { false };

        optional<RegistrationOptions> typeDefinitionProvider {};

        optional<RegistrationOptions> implementationProvider {};

        bool referencesProvider { false };

        bool documentHighlightProvider { false };

        bool documentSymbolProvider { false };

        bool workplaceSymbolProvider { false };

        optional<CodeActionOptions> codeActionProvider {};

        optional<CodeLensOptions> codeLensProvider {};

        bool documentFormattingProvider { false };

        optional<DocumentOnTypeFormattingOptions> documentOnTypeFormattingProvider {};

        optional<RenameOptions> renameProvider {};

        optional<DocumentLinkOptions> documentLinkProvider {};

        optional<ColorProviderOptions> colorProvider {};

        optional<FoldingRangeProviderOptions> foldingRangeProvider {};

        optional<ExecuteCommandOptions> executeCommandProvider {};

        optional<Workspace> workspace {};

        optional<Value> experimental {};
    };
}

#endif //LATEX_LANGUAGE_SERVER_DEFINITIONS_H
