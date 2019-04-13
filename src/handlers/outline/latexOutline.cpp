#include <lconfig.h>

#define ENV_OUTLINE_LEVEL 500

#define STRCMP(a, b) (std::strcmp(a, b) == 0)
#define NODE_NAME_IS(node, name) (STRCMP(ts_node_type(node), name))

void generateOutline (vector<DocumentSymbol> &outline, TSNode node, File &file, bool recursive, Point endPoint);

struct OutlineCommandData {
    string name;
    Point endPoint;
    bool sawStar;
};

std::optional<LConfig::OutlineSectionData> getLevelForCommandWord (const string &word) {
    const auto &sectionCommands = g_config->latex.outline.sectionCommands;
    const auto itr = sectionCommands.find(word);
    if (itr == sectionCommands.end()) { return {}; }
    return itr->second;
}

// Trim functions from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

/**
 * Gets a simplified version of the node's text.
 *
 * Math & environments are omitted,
 */
string getGroupText (TSNode node, File &file) {
    string contents {};

    uint32_t numGroupElem = ts_node_named_child_count(node) - 1;
    for (uint32_t i = 1; i < numGroupElem; i++) {
        TSNode elem = ts_node_named_child(node, i);
        if (NODE_NAME_IS(elem, "text")) {
            string text = file.utf8TextForNode(elem);
            contents += text;
        } else if (NODE_NAME_IS(elem, "control_symbol")) {
            string text = file.utf8TextForNode(elem);
            contents += '\\' + text;
        } else if (NODE_NAME_IS(elem, "control_word")) {
            string text = file.utf8TextForNode(elem);
            contents += '\\' + text;
        } else if (NODE_NAME_IS(elem, "group")) {
            string text = getGroupText(elem, file);
            contents += text;
        }
    }

    trim(contents);
    return contents;
}

void setEndPoints (DocumentSymbol &previous, Point endPoint) {
    if (previous.range.end < endPoint) { return; }

    previous.range.end = endPoint;

    if (!previous.children.empty()) {
        setEndPoints(previous.children.back(), endPoint);
    }
}

DocumentSymbol* insertOutlineEntry (vector<DocumentSymbol> &outline, DocumentSymbol &&entry) {
    if (outline.empty()) {
        return &outline.emplace_back(entry);
    }

    DocumentSymbol &last = outline.back();
    if (entry.level <= last.level) {
        setEndPoints(last, entry.range.start);
        return &outline.emplace_back(entry);
    }

    return insertOutlineEntry(last.children, std::move(entry));
}


/*
 * If an argument exists, it will be in a group node adjacent to the command.
 *
 * There may be text and comments in the way though; we check these to make sure
 * they are (1) the star, (2) whitespace, (3) not a par
 */
std::optional<OutlineCommandData> getArgumentToCommand (TSNode commandNode, File &file, bool hasStar) {
    bool s_whitespace { true };
    bool sawStar { false };

    TSNode node = ts_node_next_named_sibling(commandNode);
    while (!ts_node_is_null(node)) {

        if (NODE_NAME_IS(node, "group")) {
            string text = getGroupText(node, file);

            return OutlineCommandData {
                text,
                fromTSPoint(ts_node_end_point(node)),
                sawStar
            };
        }

        if (NODE_NAME_IS(node, "text")) {
            Point point = fromTSPoint(ts_node_start_point(node));
            auto text = file.textForNode(node);
            auto len = text.size();

            for (unsigned int i = 0; i < len; i++) {
                auto c = text[i];
                point.column++; // points to column right of char

                if (c == ' ' || c == '\t') {
                    continue;
                } else if (c == '%') {
                    s_whitespace = false;
                    while (i < len) {
                        i++;
                        if (text[i] == '\n') { break; }
                    }
                    point.row++;
                    point.column = 0;
                } else if (c == '\n') {
                    if (!s_whitespace) {
                        return {};
                    }
                    point.row++;
                    point.column = 0;
                    s_whitespace = false;
                } else if (c == '*') {
                    if (hasStar) {
                        sawStar = true;
                        hasStar = false;
                    } else {
                        return OutlineCommandData{
                            "*",
                            point,
                            sawStar
                        };
                    }
                } else {
                    return OutlineCommandData {
                            UtfHandler().utf16to8(u16string { c }),
                            point,
                            sawStar
                    };
                }
            }
        }

        node = ts_node_next_named_sibling(node);
    }

    return {};
}


/*
 * Environment node should follow a constant structure
 * (environment
 *   (open_env
 *     (begin_env)
 *     ...
 *     (group))
 *   ...))
 *
 * Comments and such should not be possible at the beginning and
 * ends, and it should never error to an environment.
 */
string getEnvironmentName (TSNode envNode, File &file) {
    assert(NODE_NAME_IS(envNode, "environment"));
    assert(ts_node_named_child_count(envNode) > 0);

    TSNode openEnvNode = ts_node_named_child(envNode, 0);
    assert(NODE_NAME_IS(openEnvNode, "open_env"));

    TSNode nameNode = ts_node_named_child(openEnvNode, ts_node_named_child_count(openEnvNode) - 1);
    assert(NODE_NAME_IS(nameNode, "group"));

    return getGroupText(nameNode, file);
}

void addControlWordOutline (vector<DocumentSymbol> &outline, TSNode child, File &file, Point endPoint) {
    string wordName = file.utf8TextForNode(child);
    auto possibleOutlineData = getLevelForCommandWord(wordName);
    if (!possibleOutlineData) { return; }

    auto &outlineData = possibleOutlineData.value();

    string name;
    Range commandRange;
    if (outlineData.hasArgument) {
        auto posArgument = getArgumentToCommand(child, file, outlineData.hasStar);
        if (posArgument) {
            auto argument = posArgument.value();
            name = argument.name;
            commandRange = Range {fromTSPoint(ts_node_start_point(child)), argument.endPoint};
        } else {
            name = "[unnamed " + wordName + "]";
            commandRange = nodeRange(child);
        }
    } else {
        name = wordName;
        commandRange = nodeRange(child);
    }

    commandRange.start.column -= 1; // `\` is not part of control sequence range
    DocumentSymbol entry = DocumentSymbol { name, commandRange, outlineData, false };
    entry.range = Range { commandRange.start, endPoint };

    insertOutlineEntry(outline, std::move(entry));
}

void addEnvOutline (vector<DocumentSymbol> &outline, TSNode child, File &file, bool recursive) {
    string name = getEnvironmentName(child, file);
    if (name == "document") {
        // We still want to search the document env, even if not recursively searching
        Point endPoint = fromTSPoint(ts_node_end_point(child)); // BUG: shouldn't contain end command
        generateOutline(outline, child, file, recursive, endPoint);
    } else {
        if (!g_config->latex.outline.includeEnvironments) { return; }

        Range beginRange = nodeRange(ts_node_named_child(child, 0));
        Range envRange = nodeRange(child);
        beginRange.start.column -= 1;
        envRange.start.column -= 1;
        auto &envOutline = *insertOutlineEntry(outline, DocumentSymbol {
                name,
                ENV_OUTLINE_LEVEL, // TODO: make configurable; possibly per environment?
                SymbolKind::Number,
                envRange,
                beginRange,
                true
        });

        if (recursive) {
            auto numEnvChildren = ts_node_named_child_count(child);
            if (numEnvChildren >= 3) {
                for (uint32_t j = 1; j < numEnvChildren - 1; j++) {
                    TSNode envChildNode = ts_node_named_child(child, j);
                    if (NODE_NAME_IS(envChildNode, "env_body")) {
                        // Contents of an environment are treated as a mini document isolated from the rest.
                        auto &envChildren = envOutline.children;
                        Point endPoint = fromTSPoint(ts_node_end_point(envChildNode));
                        generateOutline(envChildren, child, file, recursive, endPoint);
                        break;
                    }
                }
            }
        }
    }
}

void generateOutline (vector<DocumentSymbol> &outline, TSNode node, File &file, bool recursive, Point endPoint) {
    uint32_t numChildren = ts_node_named_child_count(node);
    for (uint32_t i = 0; i < numChildren; i++) {
        TSNode child = ts_node_named_child(node, i);

        if (NODE_NAME_IS(child, "control_word")) {
            addControlWordOutline(outline, child, file, endPoint);
        } else if (NODE_NAME_IS(child, "environment")) {
            addEnvOutline(outline, child, file, recursive);
        } else if (recursive) {
            generateOutline(outline, child, file, recursive, endPoint);
        }
    }
}

vector<DocumentSymbol> getOutlineForLatexFile (File &file) {
    assert(file.type == File::Type::Tex && file.hasParser);

    bool recursive = g_config->latex.outline.deepSearch;

    vector<DocumentSymbol> outline {};

    TSNode rootNode = file.getRootNode();

    generateOutline(outline, rootNode, file, recursive, file.getEndPoint());

    return outline;
}
