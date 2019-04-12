#ifndef LATEX_LANGUAGE_SERVER_COMPLETIONPROVIDER_H
#define LATEX_LANGUAGE_SERVER_COMPLETIONPROVIDER_H

#include <lsp-tools/Handler.h>

#include "range.h"

enum class PrefixType {
    None,
    TextCommand,
    MathCommand,
    Citation, // for \cite{...|}
    CitationShort, // for @...|
    MathShift,
    Magic,
    Env, // for \begin{...|}
    EnvShort, // for #...|
    Package,
    Reference,
};

struct PrefixData {
    PrefixType type { PrefixType::None };
    Range range {};
    string value {};
};

enum class InsertTextFormat {
    PlainText = 1,
    Snippet = 2
};

struct TextEdit {
    void reflect (StringWriter &writer);
    Range range {};
    string newText {};
};

struct CompletionItem { // snippet by default
    void reflect (StringWriter &writer);

    string label {};
    CompletionItemKind kind { CompletionItemKind::Snippet };
    string detail {};
    string documentation {}; // can also be markdown
    bool deprecated { false };
    InsertTextFormat format { InsertTextFormat::Snippet };
    TextEdit textEdit {};
    string sortText {};
};

struct CompletionList {
    void reflect (StringWriter &writer);

    bool empty ();

    void addSnippet (string &&prefix, string &&body, Range &range, string &&sortText = "");

    void addSnippet (string &prefix, string &body, Range &range, string &&sortText = "");

    void addCommand (string prefix, Range &range);

    void addShortEnv (const string &name, Range &range);

    void addEnvironment (string &prefix, string &envName, Range &range);

    bool isIncomplete { false }; // TODO: Support partial lists (when gathering from across files?)

    vector<CompletionItem> items {};
};

class CompletionProvider : public RequestHandler {
public:
    CompletionProvider () : RequestHandler ("textDocument/completion") {};

    void registerCapabilities (Init::ServerCapabilities &capabilities) override;

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_COMPLETIONPROVIDER_H
