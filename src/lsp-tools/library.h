#ifndef LANGUAGESERVER_TOOLS_LIBRARY_H
#define LANGUAGESERVER_TOOLS_LIBRARY_H

#include <string>
#include <map>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

#include "position.h"


/**
 * - Messages are generic valid JSON RPC messages
 * - Response is a LSP response, which must be provided an ID and a result
 * - ResponseError is a LSP response error (duh) that must be provided an ID and error
 */


using namespace std;
using namespace rapidjson;

typedef string DocumentUri;
typedef string MarkupKind;
typedef int SymbolKind;
typedef int CompletionItemKind;
typedef string CodeActionKind;

enum class FileChangeType {
    Created = 1,
    Changed = 2,
    Deleted = 3
};

struct FileEvent {
    DocumentUri uri;
    FileChangeType type;
};

class Id {
public:
    bool isIdString;

    union {
        string stringId;
        int intId;
    } id;

    bool isString () {
        return this->isIdString;
    };

    bool isInt () {
        return !this->isIdString;
    }

    int getIntID () {
        return this->id.intId;
    }

    string getStringId () {
        return this->id.stringId;
    }
};

enum class LSP_ERROR {
    // JSON RPC
    ParseError = -32700,
    InvalidRequest = -32600,
    MethodNotFound = -32601,
    InvalidParams = -32602,
    InternalError = -32603,
    serverErrorStart = -32099,
    serverErrorEnd = -32000,
    ServerNotInitialized = -32002,
    UnknownErrorCode = -32001,

    // LSP
    RequestCancelled = -32800,
    ContentModified = -32801
};

// TODO: May change; safer as string
enum class ResourceOperationKind {
    TextDocumentEdit,
    CreateFile,
    RenameFile,
    DeleteFile
};

enum class FailureHandlingKind {
    Abort,
    Transactional,
    Undo,
    TextOnlyTransactional
};

enum class MessageType {
    Error = 1,
    Warning = 2,
    Info = 3,
    Log = 4
};

enum class DiagnosticSeverity {
    ERROR = 1,
    WARNING = 2,
    INFORMATION = 3,
    HINT = 4
};

enum class Trace {
    Off,
    Messages,
    Verbose
};

struct MessageActionItem {
    string title;
};

struct Registration {
    string id;
    string method;
    void *registerOptions;
};

struct RegistrationParams {
    vector<Registration> registrations;
};

struct Unregisteration {
    string id;
    string method;
};

struct UnregisterationParams {
    vector<Unregisteration> unregisterations;
};



struct Location {
    DocumentUri uri;
    Range range;
};

struct LocationLink {
    Range originSelectionRange;
    string targetUri;
    Range targetRange;
    Range targetSelectionRange;
};

struct DiagnosticRelatedInformation {
    Location location;
    string message;
};

struct Diagnostic {
    Range range;
    DiagnosticSeverity severity;
    string code;
    string source;
    string message;
    DiagnosticRelatedInformation relatedInformation;
};

struct Command {
    string title;
    string command;
    void *arguments;
};

struct TextEdit {
    Range range;
    string newText;
};

struct TextDocumentIdentifier {
    DocumentUri uri;
};

struct VersionedTextDocumentIdentifier : TextDocumentIdentifier {
    bool isNull; // can be null when server --> client
    size_t version;
};

struct TextDocumentEdit {
    VersionedTextDocumentIdentifier textDocument;
    vector<TextEdit> edits;
};

struct CreateFileOptions {
    bool overwrite;
    bool ignoreIfExists;
};

struct CreateFile {
    string uri;
    CreateFileOptions options;
};

struct RenameFileOptions {
    bool overwrite;
    bool ignoreIfExists;
};

struct RenameFile {
    string oldUri;
    string newUri;
    RenameFileOptions options;
};

struct DeleteFileOptions {
    bool recursive;
    bool ignoreIfNotExists;
};

struct DeleteFile {
    string uri;
    DeleteFileOptions options;
};

struct EditMessage {
    ResourceOperationKind type;
    union {
        struct TextDocumentEdit textEdit;
        struct CreateFile createFile;
        struct RenameFile renameFile;
        struct DeleteFile deleteFile;
    };
};

struct WorkspaceEdit {
    map<string, TextEdit> changes;

    vector<EditMessage> documentChanges;
};

struct TextDocumentItem {
    DocumentUri uri;
    string languageId;
    size_t version;
    string text;
};

struct TextDocumentPositionParams {
    TextDocumentIdentifier textDocument;
    Point position;
};

struct ShowMessageRequestParams {
    int type;
    string message;
    vector<MessageActionItem> actions;
};

struct DocumentFilter {
    string language;
    string scheme;
    string pattern;
};

// NOTE: DocumentSelector == DocumentFilter[]

struct MarkupContent {
    MarkupKind kind;
    string value;
};

struct WorkspaceClientCapabilities {
    bool applyEdit;

    struct {
        bool documentChanges;
        vector<ResourceOperationKind> resourceOperations;
        vector<FailureHandlingKind> failureHandling;
    } workSpaceEdit;

    struct {
        bool dynamicRegistration;
    } didChangeConfiguration;

    struct {
        bool dynamicRegistration;
    } didChangeWatchedFiles;

    struct {
        bool dynamicRegistration;
        struct {
            vector<SymbolKind> valueSet;
        } symbolKind;
    } symbol;

    struct {
        bool dynamicRegistration;
    } executeCommand;

    bool workspaceFolders;

    bool configuration;
};

struct TextDocumentClientCapabilities {
    struct {
        bool dynamicRegistration;
        bool willSave;
        bool willSaveWaitUntil;
        bool didSave;
    } synchronization;

    struct {
        bool dynamicRegistration;
        struct {
            bool snippetSupport;
            bool commitCharactersSupport;
            vector<MarkupKind> documentationFormat;
            bool deprecatedSupport;
            bool preselectSupport;
        } completionItem;
        struct {
            vector<CompletionItemKind> valueSet;
        } completionItemKind;
        bool contextSupport;
    } completion;

    struct {
        bool dynamicRegistration;
        vector<MarkupKind> contentFormat;
    } hover;

    struct {
        bool dynamicRegistration;
        struct {
            vector<MarkupKind> documentationFormat;
            struct {
                bool labelOffsetSupport;
            } parameterInformation;
        } signatureInformation;
    } signatureHelp;

    struct {
        bool dynamicRegistration;
    } references;

    struct {
        bool dynamicRegistration;
    } documentHighlight;

    struct {
        bool dynamicRegistration;
        struct {
            vector<SymbolKind> valueSet;
        } symbolKind;
        bool hierarchicalDocumentSymbolSupport;
    } documentSymbol;

    struct {
        bool dynamicRegistration;
    } formatting;

    struct {
        bool dynamicRegistration;
    } rangeFormatting;

    struct {
        bool dynamicRegistration;
    } onTypeFormatting;

    struct {
        bool dynamicRegistration;
        bool linkSupport;
    } declaration;

    struct {
        bool dynamicRegistration;
        bool linkSupport;
    } definition;

    struct {
        bool dynamicRegistration;
        bool linkSupport;
    } typeDefinition;

    struct {
        bool dynamicRegistration;
        bool linkSupport;
    } implementation;

    struct {
        bool dynamicRegistration;
        struct {
            struct {
                vector<CodeActionKind> valueSet;
            } codeActionKind;
        } codeActionLiteralSupport;
    } codeAction;

    struct {
        bool dynamicRegistration;
    } codeLens;

    struct {
        bool dynamicRegistration;
    } documentLink;

    struct {
        bool dynamicRegistration;
    } colorProvider;

    struct {
        bool dynamicRegistration;
        bool prepareSupport;
    } rename;

    struct {
        bool relatedInformation;
    } publishDiagnostics;

    struct {
        bool dynamicRegistration;
        int rangeLimit;
        bool lineFoldingOnly;
    } foldingRange;
};

struct ClientCapabilities {
    WorkspaceClientCapabilities workspace;
    TextDocumentClientCapabilities textDocument;
    void *experimental;
};

struct WorkspaceFolder {
    string uri;
    string name;
};

struct InitializeParams {
    bool hasProcessID;
    size_t processID;
    DocumentUri rootUri;
    Value initializationOptions;
    ClientCapabilities capabilities;
    Trace trace = Trace::Off;
    vector<WorkspaceFolder> workspaceFolders;
};

struct SaveOptions {
    bool includeText;
};

struct TextDocumentSyncOptions {
    bool openClose;
    int change;
    bool willSave;
    bool willSaveWaitUntil;
    SaveOptions save;
};

struct StaticRegistrationOptions {
    string id;
};

struct CompletionOptions {
    bool resolveProvider;
    vector<string> triggerCharacters;
};

struct SignatureHelpOptions {
    vector<string> triggerCharacters;
};

struct TextDocumentRegistrationOptions {
    vector<DocumentFilter> documentSelector;
};

// NOTE: Supposed to be mixin of TextDocumentRegistrationOptions and StaticRegistrationOptions
struct StaticTextOptions {
    string id;
    vector<DocumentFilter> documentSelector;
};

struct CodeActionOptions {
    vector<CodeActionKind> codeActionKinds;
};

struct CodeLensOptions {
    bool resolveProvider;
};

struct DocumentOnTypeFormattingOptions {
    string firstTriggerCharacter;
    vector<string> moreTriggerCharacter;
};

struct RenameOptions {
    bool prepareProvider;
};

struct DocumentLinkOptions {
    bool resolveProvider;
};

// NOTE: This represents ColorProviderOptions & TextDocumentRegistrationOptions & StaticRegistrationOptions
typedef StaticTextOptions ColorProviderOptions;
typedef StaticTextOptions FoldingRangeProviderOptions;

struct ExecuteCommandOptions {
    vector<string> commands;
};

struct ServerCapabilities {
    TextDocumentSyncOptions textDocumentSync;
    bool hoverProvider;
    CompletionOptions completionProvider;
    SignatureHelpOptions signatureHelpProvider;
    bool definitionProvider;
    StaticTextOptions typeDefinitionProvider;
    StaticTextOptions implementationProvider;
    bool referencesProvider;
    bool documentHighlightProvider;
    bool documentSymbolProvider;
    bool workspaceSymbolProvider;
    CodeActionOptions codeActionProvider;
    CodeLensOptions codeLensProvider;
    bool documentFormattingProvider;
    bool documentRangeFormattingProvider;
    DocumentOnTypeFormattingOptions documentOnTypeFormattingProvider;
    RenameOptions renameProvider;
    DocumentLinkOptions documentLinkProvider;
    ColorProviderOptions colorProvider;
    FoldingRangeProviderOptions foldingRangeProvider;
    ExecuteCommandOptions executeCommandProvider;
    struct {
        struct {
            bool supported;
            string changeNotifications;
        } workspaceFolders;
    } workspace;
    void *experimental;
};

struct InitializeResult {
    ServerCapabilities capabilites;
};

struct ConfigurationItem {
    string scopeUri;
    string section;
};

Document getMessage ();

void sendResponse (Value &result);

void sendResponse (const string &id, Value &result);

void sendResponse (int id, Value &result);

void sendError (LSP_ERROR code, const string &message);

void sendRequest (int id, const string &method, Value &params);

void sendNotification (const string &method, Value &params);

void cancelRequest (int id);

void cancelRequest (const string &id);

void sendMessage (Document &message);

void startServerListening ();

#endif