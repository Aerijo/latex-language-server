#include <lconfig.h>
#include "../../filesystem/File.h"
#include "../../filesystem/FileManager.h"

#define STRCMP(a, b) (std::strcmp(a, b) == 0)
#define NODE_NAME_IS(node, name) (STRCMP(ts_node_type(node), name))

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

    bool ltrimNext { false };

    uint32_t numGroupElem = ts_node_named_child_count(node) - 1;
    for (uint32_t i = 1; i < numGroupElem; i++) {
        TSNode elem = ts_node_named_child(node, i);
        if (NODE_NAME_IS(elem, "text")) {
            string text = file.utf8TextForNode(elem);
            if (ltrimNext) {
                ltrimNext = false;
                ltrim(text);
            }
            contents += text;
        } else if (NODE_NAME_IS(elem, "control_sequence")) {
            string text = file.utf8TextForNode(elem);
            contents += '\\' + text;
            ltrimNext = true;
        } else if (NODE_NAME_IS(elem, "control_word")) {
            string text = file.utf8TextForNode(elem);
            contents += '\\' + text;
        } else if (NODE_NAME_IS(elem, "group")) {
            string text = getGroupText(elem, file);
            if (ltrimNext) {
                ltrimNext = false;
                ltrim(text);
            }
            contents += text;
        }
    }

    trim(contents);
    return contents;
}


/*
 * If an argument exists, it will be in a group node adjacent to the command.
 *
 * There may be text and comments in the way though; we check these to make sure
 * they are (1) the star, (2) whitespace, (3) not a par
 */
std::optional<string> getArgumentToCommand (TSNode commandNode, File &file, bool hasStar) {
    bool s_whitespace { true };

    TSNode node = ts_node_next_named_sibling(commandNode);
    while (!ts_node_is_null(node)) {

        std::cerr << ts_node_type(node) << "\n";

        if (NODE_NAME_IS(node, "group")) {
            return getGroupText(node, file);
        }

        if (NODE_NAME_IS(node, "text")) {
            auto text = file.textForNode(node);

            auto len = text.size();

            for (uint i = 0; i < len; i++) {
                auto c = text[i];
                std::cerr << c << "\n";

                if (c == ' ' || c == '\t') {
                    continue;
                } else if (c == '%') {
                    s_whitespace = false;
                    while (i < len) {
                        i++;
                        if (text[i] == '\n') { break; }
                    }
                } else if (c == '\n') {
                    if (!s_whitespace) {
                        return {};
                    }
                    s_whitespace = false;
                } else if (c == '*') {
                    if (hasStar) {
                        hasStar = false;
                    } else {
                        return "*";
                    }
                } else {
                    return UtfHandler().utf16to8(u16string {c});
                }
            }
        }

        node = ts_node_next_named_sibling(node);
    }

    return "";
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

void generateOutline (vector<DocumentSymbol> &outline, TSNode node, File &file, bool recursive) {
    uint32_t numChildren = ts_node_named_child_count(node);
    for (uint32_t i = 0; i < numChildren; i++) {
        TSNode child = ts_node_named_child(node, i);

        if (NODE_NAME_IS(child, "control_word")) {
            string wordName = file.utf8TextForNode(child);
            auto possibleOutlineData = getLevelForCommandWord(wordName);
            if (!possibleOutlineData) { continue; }

            auto &outlineData = possibleOutlineData.value();

            string name;

            if (outlineData.hasArgument) {
                auto argument = getArgumentToCommand(child, file, outlineData.hasStar);
                if (argument) {
                    name = argument.value();
                } else {
                    name = "[unnamed]";
                }
            }

            DocumentSymbol entry { name, outlineData };

            outline.emplace_back(entry);
        }

        if (recursive) {
            generateOutline(outline, child, file, recursive);
        }
    }
}

vector<DocumentSymbol> getOutlineForLatexFile (File &file) {
    assert(file.type == File::Type::Tex && file.hasParser);

    bool recursive = g_config->latex.outline.deepSearch;

    vector<DocumentSymbol> outline {};

    TSNode rootNode = file.getRootNode();

    generateOutline(outline, rootNode, file, recursive);

    return outline;
}
