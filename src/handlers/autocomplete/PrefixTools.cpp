#include "./CompletionProvider.h"

#include "../../filesystem/FileManager.h"

#define PREFIX_MAX_LENGTH 100

Point fromTSPoint (TSPoint tsPoint) {
    return Point { tsPoint.row, tsPoint.column >> 1 };
}

TSPoint toTSPoint (Point point) {
    return TSPoint { point.row, point.column << 1 };
}

Range fromTSRange (TSRange tsRange) {
    return { fromTSPoint(tsRange.start_point), fromTSPoint(tsRange.end_point) };
}

TSRange toTSRange (Range range) {
    return { toTSPoint(range.start), toTSPoint(range.end) };
}

TSRange ts_node_range (TSNode node) {
    return TSRange { ts_node_start_point(node), ts_node_end_point(node) };
}

bool isCitation (const char *type) {
    return std::strcmp(type, "cite") == 0;
}

bool isMath (const char *type) {
    return std::strcmp(type, "math") == 0;
}

bool isPackage (const char *type) {
    return std::strcmp(type, "use") == 0;
}

bool isReference (const char *type) {
    return std::strcmp(type, "ref") == 0;
}

bool isEnvironment (const char *type) {
    return std::strcmp(type, "begin") == 0;
}

TSNode getNamedChildNode(TSNode parent, const char *name) {
    auto childCount = ts_node_named_child_count(parent);
    for (uint32_t i = 0; i < childCount; i++) {
        TSNode child = ts_node_named_child(parent, i);
        if (std::strcmp(ts_node_type(child), name) == 0) return child;
    }

    return TSNode {};
}

PrefixData getEnvironmentPrefixData (File &file, Point cursorPosition, TSNode envBeginNode) {
    // this node is the entire \begin{foo}
    u16string nodeContent = file.textForNode(envBeginNode);

    TSNode nameNode = getNamedChildNode(envBeginNode, "group");
    if (ts_node_is_null(nameNode)) return {};

    TSRange nodeRange = ts_node_range(nameNode);

    Range prefixRange = { fromTSPoint(nodeRange.start_point), cursorPosition };

    return {
        PrefixType::Env,
        prefixRange,
        UtfHandler().utf16to8(file.textInRange(prefixRange))
    };
}

PrefixData getCitationPrefixData (File &file, Point cursorPosition, TSNode citationNode) {
    return {};
}

PrefixData getReferencePrefixData (File &file, Point cursorPosition, TSNode referenceNode) {
    return {};
}

PrefixData getMathPrefixData (File &file, Point cursorPosition, TSNode mathNode) {
    return {};
}

PrefixData getPackagePrefixData (File &file, Point cursorPosition, TSNode packageNode) {
    return {};
}

/*
 * Gets all letters back from the cursor to a \, !, or @.
 *
 * Alternatively gets $ or $$ if they are the first characters,
 * and will try all non space characters terminated with @ for a
 * citation key
 */
PrefixData getTextPrefix (File &file, Point cursorPosition) {
    std::cerr << "getting text prefix\n";
    unsigned i { cursorPosition.column }; // index into prefix candidate string
    unsigned startCol { 0 };
    if (cursorPosition.column > PREFIX_MAX_LENGTH) { // we only check the last PREFIX_MAX_LENGTH characters at most
        startCol = cursorPosition.column - PREFIX_MAX_LENGTH;
        i = PREFIX_MAX_LENGTH;
    }
    auto candidateText = file.textInRange({ { cursorPosition.row, startCol }, cursorPosition });

    std::cerr << "LINE: " << UtfHandler().utf16to8(candidateText) << "\n";
    std::cerr << "i: " << i << "\n";

    if (i == 0) { return { PrefixType::None }; }
    auto c = candidateText[i - 1];

    if (c == '$') {
        if (i > 1 && candidateText[i - 2] == '$') {
            return { PrefixType::MathShift, { { cursorPosition.row, cursorPosition.column - 2 }, cursorPosition }, "$$" };
        }
        return { PrefixType::MathShift, Range { Point { cursorPosition.row, cursorPosition.column - 1 }, cursorPosition }, "$" };
    }

    bool reachedStartOfLine { true };

    std::cerr << "getting line text\n";

    while (i-- != 0) {
        c = candidateText[i];
        std::cerr << (char) c << "\n";
        if (iswalpha(c)) continue;
        switch (c) {
            case ' ':
                return {};
            case '\\':
                return {
                        PrefixType::TextCommand,
                        Range { Point { cursorPosition.row, startCol + i }, cursorPosition },
                        UtfHandler().utf16to8(candidateText.substr(i, string::npos))
                };
            case '@':
                return {
                        PrefixType::CitationShort,
                        Range { Point { cursorPosition.row, startCol + i }, cursorPosition },
                        UtfHandler().utf16to8(candidateText.substr(i, string::npos))
                };
            case '#':
                return {
                        PrefixType::EnvShort,
                        Range { Point { cursorPosition.row, startCol + i }, cursorPosition },
                        UtfHandler().utf16to8(candidateText.substr(i, string::npos))
                };
            case '!':
                return {
                        PrefixType::Magic,
                        Range { Point { cursorPosition.row, startCol + i }, cursorPosition },
                        UtfHandler().utf16to8(candidateText.substr(i, string::npos))
                };
            default: {}
        }

        reachedStartOfLine = false;
        break;
    }


    if (reachedStartOfLine) return { PrefixType::None };

    while (i-- != 0) {
        switch (c) {
            case ' ':
                return { PrefixType::None };
            case '@':
                return {
                        PrefixType::Citation,
                        Range { Point { cursorPosition.row, startCol + i }, cursorPosition },
                        UtfHandler().utf16to8(candidateText.substr(i, string::npos))
                };
            default: {}
        }
        c = candidateText[i];
    }

    return { PrefixType::None };
}

vector<TSNode> getNodePathForPoint (TSNode rootNode, TSPoint point) {
    vector<TSNode> nodes {};

    TSNode node = ts_node_named_descendant_for_point_range(rootNode, point, point);

    while (!ts_node_eq(rootNode, node)) {
        nodes.push_back(node);
        node = ts_node_parent(node);
    }

    std::cerr << "returning nodes\n";
    return nodes;
}

void printNodes (vector<TSNode> &nodes) {
    std::cerr << "Nodes at cursor: ";

    if (nodes.empty()) {
        std::cerr << "No nodes at cursor\n";
        return;
    }

    for (auto i = nodes.size() - 1; i > 0; i--) {
        std::cerr << ts_node_type(nodes[i]) << " -> ";
    }

    std::cerr << ts_node_type(nodes[0]) << "\n";
}

PrefixData getPrefixData (File &file, Point cursorPosition) {
    std::cerr << "getting prefix data\n";
    vector<TSNode> nodePath = getNodePathForPoint(ts_tree_root_node(file.getParseTree()), toTSPoint(cursorPosition));
    printNodes(nodePath);

    bool seenBegin { false };

    // Nodes are most -> least specific, so the first we see should be safe enough
    for (auto node : nodePath) {
        const char *type = ts_node_type(node);
        if (isCitation(type)) {
            /*
             * - File to get text
             * - Cursor pos to know what to base suggestions on (ignore what follows)
             * - Node to get current input value
             */
            return getCitationPrefixData(file, cursorPosition, node);
        } else if (isEnvironment(type)) {
            return getEnvironmentPrefixData(file, cursorPosition, node); // want to get the group, not the begin node
        } else if (isMath(type)) {
            return getMathPrefixData(file, cursorPosition, node);
        } else if (isPackage(type)) {
            return getPackagePrefixData(file, cursorPosition, node);
        } else if (isReference(type)) {
            return getReferencePrefixData(file, cursorPosition, node);
        }
    }

    return getTextPrefix(file, cursorPosition);
}
