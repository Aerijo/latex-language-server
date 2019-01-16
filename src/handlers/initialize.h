#ifndef LATEX_LANGUAGE_SERVER_INITIALIZE_H
#define LATEX_LANGUAGE_SERVER_INITIALIZE_H

#include <Handler.h>

struct SaveOptions {
    bool includeText { false };
};

struct TextDocumentSyncOptions {
    bool openClose { false };

    TextDocumentSyncKind change { TextDocumentSyncKind::None };

    bool willSave { false };

    bool willSaveWaitUntil { false };

    SaveOptions save {};
};

struct CompletionOptions {
    bool resolveProvider { false };

    vector<string> triggerCharacters {};
};

struct SignatureHelpOptions {
    vector<string> triggerCharacters {};
};

struct DocumentFilter {
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

struct RegistrationOptions : StaticRegistrationOptions, TextDocumentRegistrationOptions {

};

struct CodeActionOptions {
    vector<CodeActionKind> codeActionKinds {};
};

struct CodeLensOptions {
    bool resolveProvider { false };
};

struct DocumentOnTypeFormattingOptions {
    string firstTriggerCharacter {};

    vector<string> moreTriggerCharacter {};
};

struct RenameOptions {
    bool prepareProvider { false };
};

struct DocumentLinkOptions {
    bool resolveProvider { false };
};

struct ColorProviderOptions {
};

struct FoldingRangeProviderOptions {
};

struct ExecuteCommandOptions {
    vector<string> commands {};
};

struct WorkspaceFolders {
    bool supported { false };

    string changeNotifications {};
};

struct Workspace {
    WorkspaceFolders workspaceFolders {};
};

struct ServerCapabilities {
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

    Workspace workspace {};

    optional<Value> experimental {};
};

class InitializeHandler : public RequestHandler {
public:
    InitializeHandler () : RequestHandler ("initialize") {};

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_INITIALIZE_H
