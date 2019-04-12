#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <lconfig.h>

using std::string;
using std::vector;
using std::unordered_map;

namespace fs = std::filesystem;

typedef LConfig::LatexConfig::CWL::CWLDef CWLDef;

vector<string> getFileContents (const string &path) {
    std::ifstream fileStream (path);
    vector<string> lines;
    string line;
    if (fileStream.is_open()) {
        while (getline(fileStream, line)) {
            lines.emplace_back(line);
        }
    }
    fileStream.close();
    return lines;
}

vector<string> extractCommands (vector<string> &lines) {
    vector<string> commands {};

    for (size_t i = 0, len = lines.size(); i < len; i++) {
        string line = lines[i];

        if (line.rfind("#keyvals", 0) == 0) {
            for (; i < len; i++) {
                if (lines[i].rfind("#endkeyvals", 0) == 0) {
                    break;
                }
            }
        } else if (line[0] == '\\') {
            commands.emplace_back(line);
        }
    }

    return commands;
}

bool extractSpecialPlaceholder (string &snippet, string &line, size_t &i, int &placeNum) {
    i++;
    auto start = i;
    for (; i < line.size(); i++) {
        if (line[i] == '%') {
            if (i < line.size() - 1 && line[i + 1] == '>') {
                string placeholder = line.substr(start, i - start);
                snippet += "${" + std::to_string(++placeNum) + ":" + placeholder + "}";
                i++;
                return true;
            }
        }
    }

    return false;
}

void extractSpecial (string &snippet, string &line, size_t &i, int &placeNum) {
    i++;
    if (i == line.size()) { return; }
    switch (line[i]) {
        case '\\':
            snippet += '\n';
            break;
        case '|':
            snippet += "$" + std::to_string(++placeNum);
            break;
        case '<':
            extractSpecialPlaceholder(snippet, line, i, placeNum);
            break;
        default:
            break;
    }
}

void extractArgument (string &snippet, string &line, size_t &i, int &placeNum, char end) {
    i++;
    auto start = i;
    string argument;
    bool foundArgument { false };

    for (; i < line.size(); i++) {
        if (line[i] == end) { break; }
        if (line[i] == '%') {
            bool isSpecial { false };
            argument = line.substr(start, i - start);

            if (i < line.size() - 1 && line[i + 1] == '<') {
                i++;
                isSpecial = extractSpecialPlaceholder(argument, line, i, placeNum);
            }

            if (!isSpecial) {
                snippet += "${" + std::to_string(++placeNum) + ":" + argument + "}";
            } else {
                snippet += argument;
            }

            foundArgument = true;
        }
    }

    if (!foundArgument) {
        argument = line.substr(start, i - start);
        snippet += "${" + std::to_string(++placeNum) + ":" + argument + "}";
    }
}

std::optional<std::pair<string, string>> convertToSnippet (string &line) {
    size_t i = 1;
    for (; i < line.size(); i++) {
        if (!isalpha(line[i])) { break; }
    }

    string commandName = line.substr(0, i);

    if (commandName == "\\begin") { return {}; } // TODO: extract environments

    string snippet = "\\" + commandName;

    int placeNum = 0;

    bool cont { true };

    for (; i < line.size() && cont; i++) {
        switch (line[i]) {
            case '{':
                snippet += "{";
                extractArgument(snippet, line, i, placeNum, '}');
                snippet += "}";
                break;
            case '[':
                snippet += "[";
                extractArgument(snippet, line, i, placeNum, ']');
                snippet += "]";
                break;
            case '%':
                extractSpecial(snippet, line, i, placeNum);
                break;
            case '#':
                // end of definition (metadata follows)
                if (i < line.size() - 1 && line[i + 1] == 'S') { return {}; }
                cont = false;
                break;
            case '\\':
                snippet += R"|(\\\\)|";
                break;
            default:
                snippet += line[i];
        }
    }

    snippet += "$0";

    return std::pair { commandName, snippet };
}

vector<std::pair<string, string>> extractSnippets (vector<string> &commands) {
    unordered_set<string> prefixes {};
    vector<std::pair<string, string>> snippets {};
    snippets.reserve(commands.size() / 2);

    for (auto &command : commands) {
        auto posSnippet = convertToSnippet(command);
        if (posSnippet && prefixes.count(posSnippet.value().first) == 0) {
            prefixes.insert(posSnippet.value().first);
            snippets.emplace_back(posSnippet.value());
        }
    }
    return snippets;
}

void processCommands (CWLDef &data, vector<string> &commands) {
    bool includeOptional = g_config->latex.cwl.includeOptional;
    bool includePlaceholders = g_config->latex.cwl.includePlaceholders;

    unordered_set<string> prefixes {}; // so we don't add duplicates
}

CWLDef parseCWL (const string &path) {
    CWLDef data {};
    auto lines = getFileContents(path);
    auto commands = extractCommands(lines);
    processCommands(data, commands);
    return data;
}

string locateCWLDirectory () {
    return "/home/benjamin/github/latex-language-server/third_party/LaTeX-cwl";
}

void locateAndBuildCWLFiles () {
    fs::path directory ( locateCWLDirectory() );

    std::cerr << "CWD: " << fs::current_path() << "\n";
    std::cerr << "CWL: " << directory << "\n";

    if (!fs::exists(directory)) return;

    vector<string> cwlFilePaths {};
    for (auto &p : fs::directory_iterator(directory)) {
        if (p.path().extension() == ".cwl") {
            cwlFilePaths.emplace_back(p.path());
        }
    }

    for (auto &file : cwlFilePaths) {
        auto snippets = parseCWL(file);
        string package = fs::path(file).filename();
        package.erase(package.size() - 4); // remove extension
        g_config->latex.cwl.cwlFiles.insert({package, snippets});
    }

    g_config->latex.cwl.initialised = true;
}

unordered_map<string, CWLDef> *getCWLFiles () {
    if (!g_config->latex.cwl.initialised) {
        locateAndBuildCWLFiles();
    }

    return &g_config->latex.cwl.cwlFiles;
}
