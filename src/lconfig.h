#ifndef LATEX_LANGUAGE_SERVER_CONFIG_H
#define LATEX_LANGUAGE_SERVER_CONFIG_H

#include <string>
#include <vector>

#include "biber/Style.h"

#include <rapidjson/document.h>

#include "lsp-tools/definitions.h"

using std::string;
using std::vector;

using rapidjson::Value;

namespace LConfig {

    struct DocumentUri {
        DocumentUri () = default;

        explicit DocumentUri (const char *uri) : uri { uri } {}

        const char *uri;

    };

    /**
     * Holds information about a section name, such as if
     * it has a starred variant, if it takes an argument,
     * what symbol to use, what level it is.
     */
    struct OutlineSectionData {
        int level;
        int symbol;
        bool hasStar { true };
        bool hasArgument { true };

        OutlineSectionData (int level, int symbol, bool hasStar=true, bool hasArgument=true)
            : level { level }, symbol { symbol }, hasStar { hasStar }, hasArgument { hasArgument } {}
    };

    namespace OutlineSymbol {
    /**
 * Convenient way to set the default section symbol kinds.
 */
    enum {
        Part = static_cast<int>(SymbolKind::File),
        Chapter = static_cast<int>(SymbolKind::Module),
        Section = static_cast<int>(SymbolKind::Namespace),
        Subsection = static_cast<int>(SymbolKind::Package),
        Subsubsection = static_cast<int>(SymbolKind::Function),
        Paragraph = static_cast<int>(SymbolKind::Field),
        Subparagraph = static_cast<int>(SymbolKind::Null),
    };
    }

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
        struct Outline {
            bool extendedSymbolKinds { false }; // if special symbols for outline view are supported by client

            bool includeEnvironments { true }; // add environment names and contents to tree

            bool deepSearch { true }; // search recursively through groups and environments, instead of just top level

            std::unordered_map<string, OutlineSectionData> sectionCommands {
                    {"part", {-1, OutlineSymbol::Part}},
                    {"chapter", {0, OutlineSymbol::Chapter}},
                    {"section", {1, OutlineSymbol::Section}},
                    {"subsection", {2, OutlineSymbol::Subsection}},
                    {"subsubsection", {3, OutlineSymbol::Subsubsection}},
                    {"paragraph", {4, OutlineSymbol::Paragraph}},
                    {"subparagraph", {5, OutlineSymbol::Subparagraph}},
            };

        } outline;

        struct CWL {
            bool initialised { false };

            bool includePlaceholders { true };

            bool includeOptional { true };

            bool prioritisedSuggestions { true }; // hides suggestions marked by #*

            enum class EnvKind {
                General, // is generic environment / command valid anywhere
                Math,    // is math env / math commands only show here
                NotMath, // command appears anywhere except math env
                Tabular, // is tabular env / tabular commands appear here
                Tabbing, // .
                Verbatim, // .
                Theorem, // .
            };

            enum class CommandKind {
                Generic,
                Usepackage, // u
                Reference, // r
                Citation, // c; & C for complex citation (TODO)
                Label, // l
                Definition, // d
                Includegraphics, // g
                Includefile, // i
                Bibliography, // b
                Url, // U
                Bracketlike, // K; e.g., \big{
                Todo, // D
                Color, // B
            };

            struct CWLSnippet {
                string prefix;

                string snippet;

                EnvKind context;
            };

            struct CWLDef {
                vector<EnvKind, vector<CWLSnippet>> snippets {}; // snippets sorted by valid context

                unordered_map<string, EnvKind> environments {}; // environment names & their kind

                unordered_map<string, CommandKind> commands {}; // map command name to kind
            };

            unordered_map<string, CWLDef> cwlFiles {};

            unordered_set<string> globalEnabled {"latex-document"};
        } cwl;
    };

    struct BibConfig {
        Style::Style *style { nullptr };

        ~BibConfig () {
            delete style;
        }
    };

}; // end namespace Config

struct GlobalConfig {
    LConfig::LspConfig lsp {};
    LConfig::LatexConfig latex {};
    LConfig::BibConfig bibtex {};
};

extern GlobalConfig *g_config;

#endif //LATEX_LANGUAGE_SERVER_CONFIG_H
