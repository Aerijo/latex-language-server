#ifndef LATEX_LANGUAGE_SERVER_OUTLINEHANDLER_H
#define LATEX_LANGUAGE_SERVER_OUTLINEHANDLER_H

#include <range.h>
#include <lconfig.h>
#include "../../lsp-tools/Handler.h"

struct SectionData {
    int level { 0 };

    size_t index { 0 }; // for building tree

    bool starred { false };

    SymbolKind kind { SymbolKind::Class };

    string name {};

    Range selectionRange {};

    explicit SectionData (Range range) : selectionRange { range } {}
};

struct DocumentSymbol {
    void reflect (StringWriter &writer);

    DocumentSymbol (string name, SymbolKind kind, Range range, Range selectionRange) : name { name }, kind { kind }, range { range }, selectionRange { selectionRange } {};

    explicit DocumentSymbol (SectionData data) : name { data.name }, kind { data.kind }, range { data.selectionRange }, selectionRange { data.selectionRange } {}

    DocumentSymbol (string name, LConfig::OutlineSectionData data) : name { name }, kind { data.symbol } {}

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
