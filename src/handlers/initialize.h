#ifndef LATEX_LANGUAGE_SERVER_INITIALIZE_H
#define LATEX_LANGUAGE_SERVER_INITIALIZE_H

#include <string>
#include <vector>
#include <optional>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

#include "definitions.h"
#include "Handler.h"

using std::string;
using std::vector;
using std::optional;

using rapidjson::Writer;
using rapidjson::StringBuffer;
using rapidjson::Value;

namespace Init {

    struct Reflectable {
        virtual void reflect (StringWriter &writer) = 0;
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
            reflectObject(writer, name, value.value());
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
            reflectVector<string>(writer, "triggerCharacters", triggerCharacters);
            writer.EndObject();
        }

        bool resolveProvider { false };

        vector<string> triggerCharacters {};
    };

    struct SignatureHelpOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectVector<string>(writer, "triggerCharacters", triggerCharacters);
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

    struct TextDocumentRegistrationOptions : Reflectable {
        void reflect (StringWriter &writer) override {
            writer.StartObject();
            reflectVector<DocumentFilter>(writer, "documentSelector", documentSelector);
            writer.EndObject();
        }

        vector<DocumentFilter> documentSelector {};
    };

    struct StaticRegistrationOptions {
        string id {};
    };

    struct RegistrationOptions : StaticRegistrationOptions, TextDocumentRegistrationOptions {

    };

    struct CodeActionOptions : Reflectable {
        void reflect (StringWriter &writer) override {

        }

        vector<CodeActionKind> codeActionKinds {};
    };

    struct CodeLensOptions : Reflectable {
        void reflect (StringWriter &writer) override {

        }

        bool resolveProvider { false };
    };

    struct DocumentOnTypeFormattingOptions : Reflectable {
        void reflect (StringWriter &writer) override {

        }

        string firstTriggerCharacter {};

        vector<string> moreTriggerCharacter {};
    };

    struct RenameOptions : Reflectable {
        void reflect (StringWriter &writer) override {

        }

        bool prepareProvider { false };
    };

    struct DocumentLinkOptions : Reflectable {
        void reflect (StringWriter &writer) override {

        }

        bool resolveProvider { false };
    };

    struct ColorProviderOptions : Reflectable {
        void reflect (StringWriter &writer) override {

        }
    };

    struct FoldingRangeProviderOptions : Reflectable {
        void reflect (StringWriter &writer) override {

        }
    };

    struct ExecuteCommandOptions : Reflectable {
        void reflect (StringWriter &writer) override {

        }

        vector<string> commands {};
    };

    struct WorkspaceFolders : Reflectable {
        void reflect (StringWriter &writer) override {

        }

        bool supported { false };

        string changeNotifications {};
    };

    struct Workspace : Reflectable {
        void reflect (StringWriter &writer) override {

        }

        WorkspaceFolders workspaceFolders {};
    };

    struct ServerCapabilities : Reflectable {
        void reflect (StringWriter &writer) override {
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

class InitializeHandler : public RequestHandler {
public:
    InitializeHandler () : RequestHandler ("initialize") {};

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_INITIALIZE_H
