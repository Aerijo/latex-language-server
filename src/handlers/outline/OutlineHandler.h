#ifndef LATEX_LANGUAGE_SERVER_OUTLINEHANDLER_H
#define LATEX_LANGUAGE_SERVER_OUTLINEHANDLER_H

#include <range.h>
#include <lconfig.h>
#include "../../lsp-tools/Handler.h"

struct DocumentSymbol {
    void reflect (StringWriter &writer);

    DocumentSymbol (string name, int level, SymbolKind kind, Range range, Range selectionRange) : level { level }, name { name }, kind { kind }, range { range }, selectionRange { selectionRange } {};

    DocumentSymbol (string name, Range commandRange, LConfig::OutlineSectionData data) : level { data.level }, name { name }, kind { data.symbol }, selectionRange { commandRange } {}

    int level;

    string name;

    string detail {};

    SymbolKind kind;

    bool deprecated { false };

    Range range;

    Range selectionRange;

    vector<DocumentSymbol> children {};
};

class OutlineHandler : public RequestHandler {
public:
    OutlineHandler () : RequestHandler ("textDocument/documentSymbol") {};

    void registerCapabilities (Init::ServerCapabilities &capabilities) override;

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_OUTLINEHANDLER_H
