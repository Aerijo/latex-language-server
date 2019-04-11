struct EntryData {
    Range range;
    Range selectionRange;
    string name { "[no type]" };
    string key { "[no key]" };
    SymbolKind kind { SymbolKind::Variable };

    explicit EntryData (Range range) : range { range } {}
};

void setEntryKind (EntryData &data) {
    if (data.name == "book") {
        data.kind = SymbolKind::File;
    }
}

EntryData getEntryData (TSNode entryNode, File &file) {
    assert(strcmp(ts_node_type(entryNode), "entry") == 0);

    EntryData data { nodeRange(entryNode) };

    auto numChildren = ts_node_named_child_count(entryNode);
    for (uint32_t i = 0; i < numChildren; i++) {
        TSNode child = ts_node_named_child(entryNode, i);
        const char *type = ts_node_type(child);
        if (strcmp(type, "name") == 0) {
            data.name = file.utf8TextForNode(child);
            data.selectionRange = nodeRange(child);
            setEntryKind(data);
        } else if (strcmp(type, "key") == 0) {
            data.key = file.utf8TextForNode(child);
            return data;
        }
    }

    return data;
}

void addEntryData (vector<DocumentSymbol> &symbols, EntryData &data) {
    symbols.emplace_back(DocumentSymbol(data.name + ": " + data.key, 0, data.kind, data.range, data.selectionRange));
}

vector<DocumentSymbol> getOutlineForBibFile (File &file) {
    vector<DocumentSymbol> symbols {};

    std::cerr << "getting bib outline\n";

    TSNode rootNode = file.getRootNode();
    auto numChildren = ts_node_named_child_count(rootNode);
    for (uint32_t i = 0; i < numChildren; i++) {
        TSNode child = ts_node_named_child(rootNode, i);
        auto type = ts_node_type(child);
        if (strcmp(type, "entry") == 0) {
            EntryData entryData = getEntryData(child, file);
            addEntryData(symbols, entryData);
        }
    }

    return symbols;
}
