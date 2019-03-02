#include <lconfig.h>

#define OUTLINE_ENVIRONMENTS true

struct SectionPathNode {
    DocumentSymbol *symbol;

    int level;

    enum class Kind {
        Section,
        Env
    } kind;

    SectionPathNode (DocumentSymbol *symbol, int level, Kind kind = Kind::Section) : symbol { symbol }, level { level }, kind { kind } {}
};

int getLevelForSectionCSNode (TSNode node, File &file) {
    auto commandName = file.textForNode(node);

    if (commandName == u"\\section") {
        return 2;
    } else if (commandName == u"\\subsection") {
        return 3;
    } else if (commandName == u"\\chapter") {
        return 1;
    } else if (commandName == u"\\part") {
        return 0;
    } else if (commandName == u"\\subsubsection") {
        return 4;
    } else if (commandName == u"\\paragraph") {
        return 5;
    } else if (commandName == u"\\subparagraph") {
        return 6;
    }

    return 2; // default to section level
}

string getGroupNodeText (TSNode group, File &file) {
    string nodeText {};
    // TODO: Make better -_- (whitespace between nodes will be ignored currently)
    // first node is $.l, the rest is arbitrary until end with $.r
    auto numChildren = ts_node_named_child_count(group);
    for (uint32_t i = 1; i < numChildren - 1; i++) {
        TSNode child = ts_node_child(group, i);
        auto type = ts_node_type(child);
        if (strcmp(type, "comment") == 0) continue;
        nodeText += file.utf8TextForNode(child);
    }

    return nodeText;
}

SymbolKind symbolKindForSectionLevel (int level) {
    if (g_config->latex.extensions.headingSymbolKinds) {
        return SymbolKind::Property; // TODO
    } else {
        switch (level) {
            case 0: return SymbolKind::File;
            case 1: return SymbolKind::Module;
            case 2: return SymbolKind::Namespace;
            case 3: return SymbolKind::Package;
            case 4: return SymbolKind::Function;
            case 5: return SymbolKind::Field;
            default: return SymbolKind::Null;
        }
    }
}

SectionData getSectionData (TSNode sectionNode, File &file) {
    assert(strcmp(ts_node_type(sectionNode), "section") == 0);

    SectionData sectionData { nodeRange(sectionNode) };

    auto numChildren = ts_node_named_child_count(sectionNode);
    for (uint32_t i = 0; i < numChildren; i++) {
        TSNode child = ts_node_named_child(sectionNode, i);
        auto type = ts_node_type(child);
        if (strcmp(type, "cs") == 0) {
            sectionData.level = getLevelForSectionCSNode(child, file);
            sectionData.kind = symbolKindForSectionLevel(sectionData.level);
        } else if (strcmp(type, "star") == 0) {
            sectionData.starred = true;
        } else if (strcmp(type, "group") == 0) {
            sectionData.name = getGroupNodeText(child, file);
        }
    }

    if (sectionData.name.empty()) {
        sectionData.name = "[unnamed section]";
    }

    return sectionData;
}

void addSectionData (vector<DocumentSymbol> &sections, vector<SectionPathNode> &treePath, SectionData &data) {
    // need to set the range of the previous sections & add to tree path & add to sections
    data.index = sections.size();

    auto i = treePath.size();

    if (i == 0) { // if this is the first heading
        auto &outline = sections.emplace_back(DocumentSymbol(data));
        treePath.emplace_back(SectionPathNode(&outline, data.level));
        return;
    }

    while (true) {
        i--;

        auto &pathNode = treePath[i];
        auto level = pathNode.level;

        if (data.level > level) { // this is nested under the current path tree
            DocumentSymbol &outlineNode = *pathNode.symbol;
            auto &nestedOutline = outlineNode.children.emplace_back(DocumentSymbol(data));
            treePath.emplace_back(SectionPathNode(&nestedOutline, data.level));
            return;
        } else if (data.level == level) { // this is at same level as previous section command
            DocumentSymbol &outlineNode = *pathNode.symbol;
            outlineNode.range.end = data.selectionRange.start; // set the end range of the prev section

            treePath.pop_back();
            DocumentSymbol *outline;
            outline = (i == 0)
                ? &sections.emplace_back(DocumentSymbol(data)) // is root level if the previous was too
                : &treePath[i - 1].symbol->children.emplace_back(DocumentSymbol(data));
            treePath.emplace_back(SectionPathNode(outline, data.level));
            return;
        } else { // this is a greater section than the current one at the end of the path
            DocumentSymbol &outlineNode = *pathNode.symbol;
            outlineNode.range.end = data.selectionRange.start;
            treePath.pop_back();

            if (i == 0) { // if we have reached the root, this becomes a de facto main heading
                auto &outline = sections.emplace_back(DocumentSymbol(data));
                treePath.emplace_back(SectionPathNode(&outline, data.level));
                return;
            }
        }
    }
}

void setEndpointsOfLastSymbols (vector<DocumentSymbol> &symbols, Point endPoint) {
    if (symbols.empty()) return;

    DocumentSymbol *last = &symbols.back();
    last->range.end = endPoint;

    while (!last->children.empty()) {
        last = &last->children.back();
        last->range.end = endPoint;
    }
}

void gatherOutlineForNode (TSNode node, File &file, vector<DocumentSymbol> &symbols, vector<SectionPathNode> &treePath) {
    auto numChildren = ts_node_named_child_count(node);
    for (uint32_t i = 0; i < numChildren; i++) {
        TSNode child = ts_node_named_child(node, i);
        auto type = ts_node_type(child);
        if (strcmp(type, "section") == 0) {
            SectionData sectionData = getSectionData(child, file);
            addSectionData(symbols, treePath, sectionData);
        } else {
            gatherOutlineForNode(child, file, symbols, treePath);
        }
    }
}

vector<DocumentSymbol> getOutlineForLatexFile (File &file) {
    // TODO: Environments, external files, values for custom symbols

    vector<DocumentSymbol> symbols {};
    vector<SectionPathNode> treePath {};

    TSNode rootNode = file.getRootNode();
    Point endPoint = file.getEndPoint();

    gatherOutlineForNode(rootNode, file, symbols, treePath);

    setEndpointsOfLastSymbols(symbols, endPoint);

    return symbols;
}
