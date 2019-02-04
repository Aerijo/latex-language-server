#ifndef LATEX_LANGUAGE_SERVER_CONFIG_H
#define LATEX_LANGUAGE_SERVER_CONFIG_H

#include <string>
#include <vector>

#include "biber/Style.h"

#include <rapidjson/document.h>

#include "lsp-tools/definitions.h"

//using std::optional;
using std::string;
using std::vector;

using rapidjson::Value;

namespace LConfig {

    struct DocumentUri {
        DocumentUri () = default;

        explicit DocumentUri (const char *uri) : uri(uri) {}

        const char *uri;

    };

// TODO
    enum ResourceOperationKind {

    };

// TODO
    enum FailureHandlingKind {

    };

    struct WorkspaceEdit {
        bool documentChanges { false };

        vector<ResourceOperationKind> resourceOperations;

        FailureHandlingKind failureHandling;
    };

    struct DidChangeConfiguration {
        bool dynamicRegistration { false };
    };

    struct DidChangeWatchedFiles {
        bool dynamicRegistration { false };
    };

    struct Symbol {
        bool dynamicRegistration { false };

        vector<SymbolKind> symbolKindValueSet {};
    };

    struct ExecuteCommand {
        bool dynamicRegistration { false };
    };

    struct WorkspaceClientCapabilities {
        bool applyEdit { false };

        WorkspaceEdit workspaceEdit {};

        DidChangeConfiguration didChangeConfiguration {};

        DidChangeWatchedFiles didChangeWatchedFiles {};

        Symbol symbol {};

        ExecuteCommand executeCommand {};

        bool workspaceFolders { false };

        bool configuration { false };
    };

    struct Synchronization {
        bool dynamicRegistration { false };

        bool willSave { false };

        bool willSaveWaitUntil { false };

        bool didSave { false };
    };

    struct CompletionItem {
        bool snippetSupport { false };

        bool commitCharactersSupport { false };

        vector<MarkupKind> documentationFormat {};

        bool deprecatedSupport { false };

        bool preselectSupport { false };
    };

    struct Completion {
        bool dynamicRegistration {};

        CompletionItem completionItem {};

        vector<CompletionItemKind> completionItemKindValueSet {};

        bool contextSupport { false };
    };

    struct Hover {
        bool dynamicRegistration { false };

        vector<MarkupKind> contentFormat {};
    };

    struct ParameterInformation {
        bool labelOffsetSupport { false };
    };

    struct SignatureInformation {
        vector<MarkupKind> documentationFormat {};

        ParameterInformation parameterInformation {};
    };

    struct SignatureHelp {
        bool dynamicRegistration { false };

        SignatureInformation signatureInformation {};
    };

    struct References {
        bool dynamicRegistration { false };
    };

    struct DocumentHighlight {
        bool dynamicRegistration { false };
    };

    struct DocumentSymbol {
        bool dynamicRegistration { false };

        vector<SymbolKind> symbolKindValueSet {};

        bool hierarchicalDocumentSymbolSupport { false };
    };

    struct Formatting {
        bool dynamicRegistration { false };
    };

    struct RangeFormatting {
        bool dynamicRegistration { false };
    };

    struct OnTypeFormatting {
        bool dynamicRegistration { false };
    };

    struct Declaration {
        bool dynamicRegistration { false };

        bool linkSupport { false };
    };

    struct Definition {
        bool dynamicRegistration { false };

        bool linkSupport { false };
    };

    struct TypeDefinition {
        bool dynamicRegistration { false };

        bool linkSupport { false };
    };

    struct Implementation {
        bool dynamicRegistration { false };

        bool linkSupport { false };
    };

    struct CodeActionLiteralSupport {
        vector<CodeActionKind> codeActionKindValueSet {};
    };

    struct CodeAction {
        bool dynamicRegistration { false };

        CodeActionLiteralSupport codeActionLiteralSupport {};
    };

    struct CodeLens {
        bool dynamicRegistration { false };
    };

    struct DocumentLink {
        bool dynamicRegistration { false };
    };

    struct ColorProvider {
        bool dynamicRegistration { false };
    };

    struct Rename {
        bool dynamicRegistration { false };

        bool prepareSupport { false };
    };

    struct PublishDisgnostics {
        bool relatedInformation { false };
    };

    struct FoldingRange {
        bool dynamicRegistration { false };

        int rangeLimit { -1 };

        bool lineFoldingOnly { false };
    };

    struct TextDocumentClientCapabilities {
        Synchronization synchronization {};

        Completion completion {};

        Hover hover {};

        SignatureHelp signatureHelp {};

        References references {};

        DocumentHighlight documentHighlight;

        DocumentSymbol documentSymbol {};

        Formatting formatting {};

        RangeFormatting rangeFormatting {};

        OnTypeFormatting onTypeFormatting {};

        Declaration declaration {};

        Definition definition {};

        TypeDefinition typeDefinition {};

        Implementation implementation {};

        CodeAction codeAction {};

        CodeLens codeLens {};

        DocumentLink documentLink {};

        ColorProvider colorProvider {};

        Rename rename {};

        PublishDisgnostics publishDisgnostics {};

        FoldingRange foldingRange {};
    };

    struct ClientCapabilities {
        WorkspaceClientCapabilities workspace {};

        TextDocumentClientCapabilities textDocument {};

        Value *experimental { nullptr };
    };

    struct WorkspaceFolder {
        string uri {};

        string name {};
    };

    struct LspConfig {
        optional<pid_t> processId {};

        // @deprecated rootPath; use rootUri

        optional<DocumentUri> rootUri {};

        optional<Value *> initializationOptions {};

        ClientCapabilities capabilities {};

        string trace { "off" };

        optional<vector<WorkspaceFolder>> workspaceFolders {};
    };

    struct LatexConfig {

    };

    struct BibConfig {
        Style::Style *style { nullptr };
    };

}; // end namespace Config

struct GlobalConfig {
    LConfig::LspConfig lsp {};
    LConfig::LatexConfig latex {};
    LConfig::BibConfig bibtex {};

    ~GlobalConfig () {
        delete bibtex.style;
    }
};

extern GlobalConfig *g_config;

#endif //LATEX_LANGUAGE_SERVER_CONFIG_H
