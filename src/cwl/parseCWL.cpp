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

typedef LConfig::LatexConfig::CWL CWL;
typedef CWL::CWLDef CWLDef;

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

void skipUntil (string &command, size_t &i, char end) {
    i = command.find(end, i);
    if (i == string::npos) {
        i = command.size();
    }
}

void processEnvironment (string &command, string &classification, CWLDef &data) {
    auto envNameEnd = command.find('}');
    if (envNameEnd == string::npos) { return; }
    string envName = command.substr(7, envNameEnd - 7); // command == \begin{name}

    auto typeStart = classification.find('\\');
    if (typeStart == string::npos) {
        data.environments.insert({envName, CWL::EnvKind::General});
    } else {
        string type {};
        auto i = ++typeStart;
        for (; i < classification.size(); i++) {
            if (!isalpha(classification[i])) { break; }
        }

        type = classification.substr(typeStart, i - typeStart);

        if (type == "math") {
            data.environments.insert({envName, CWL::EnvKind::Math});
        } else if (type == "tabular") {
            data.environments.insert({envName, CWL::EnvKind::Tabular});
        } else if (type == "array") {
            data.environments.insert({envName, CWL::EnvKind::Array});
        } else {
            data.environments.insert({envName, CWL::EnvKind::General});
        }
    }
}

void extractArgument (string &snippet, string &command, size_t &i, char end, int &place, bool includePlacholders) {
    auto start = i;
    skipUntil(command, i, end);

    if (!includePlacholders) {
        snippet += "$" + std::to_string(++place);
        return;
    }

    string arg = command.substr(start, i - start);

    auto special = arg.find('%');
    if (special == string::npos || special == i - 1) {
        snippet += "${" + std::to_string(++place) + ":" + arg + "}";
        return;
    }

    snippet += "${" + std::to_string(++place) + ":" + command.substr(special, i - special) + "}";
}

string extractSnippet (string &command, bool includeOptional, bool includePlaceholders) {
    // TODO: Proper % special support

    string snippet {};
    string arg;
    int placeNum = 0;
    for (size_t i = 0; i < command.size(); i++) {
        auto c = command[i];
        switch (c) {
            case '{':
                snippet += '{';
                extractArgument(snippet, command, ++i, '}', placeNum, includePlaceholders);
                snippet += '}';
                break;
            case '(':
                snippet += '(';
                extractArgument(snippet, command, ++i, ')', placeNum, includePlaceholders);
                snippet += ')';
                break;
            case '[':
                if (includeOptional) {
                    snippet += '[';
                    extractArgument(snippet, command, ++i, ']', placeNum, includePlaceholders);
                    snippet += ']';
                } else {
                    skipUntil(command, i, ']');
                }
                break;
            case '\\':
                snippet += "\\\\";
                break;
            default:
                snippet += c;
        }
    }

    snippet += "$0";

    return snippet;
}

enum class ClassKind {
    None,
    Math,
    Tabular,
    Tabbing,
    Text,
};

enum class DefKind {
    None,
    Reference,
    Citation,
    Label,
    Definition,
    Graphics,
    File,
    Package,
    Bibliography,
    Url,
    Bracketlike,
    Todo,
    Color,
    Special,
    Verbatim,
    Newtheorem,
    Structure,
};

struct Classification {
    bool extended { false }; // *
    bool visible { true }; // S

    ClassKind classKind { ClassKind::None };
    DefKind defKind { DefKind::None };
    int sectionLevel { 0 };
    vector<string> validEnvs {};
    string behavesLike {};
};

void extractStructureLevel (Classification &result, string &c, size_t &i) {
    auto start = i;
    if (c[i] == '-') { i++; }
    for (; i < c.size(); i++) {
        if (!isnumber(c[i])) { i--; break; } // i will be incremented after
    }

    string level = c.substr(start, i - start);
    if (level.empty()) { return; }

    result.sectionLevel = std::stoi(level) - 1;
}

void extractValidEnvs (Classification &result, string &c, size_t &i) {
    auto start = i;
    string envName;
    for (; i < c.size(); i++) {
        if (c[i] != ',') { continue; }
        envName = c.substr(start, i - start);
        if (!envName.empty()) { result.validEnvs.push_back(envName); }
        start = i + 1;
    }
    envName = c.substr(start, i - start);
    if (!envName.empty()) { result.validEnvs.push_back(envName); }
}

void extractLikeEnv (Classification &result, string &c, size_t &i) {
    result.behavesLike = c.substr(i, c.size() - i);
    i = c.size();
}

Classification identifyClassfification (string &classification) {
    Classification result {};

    for (size_t i = 0; i < classification.size(); i++) {
        auto c = classification[i];
        switch (c) {
            case '*': result.extended = true; break;
            case 'S': result.visible = false; break;
            case 'M': break; // TODO
            case 'm': result.classKind = ClassKind::Math; break;
            case 't': result.classKind = ClassKind::Tabular; break;
            case 'T': result.classKind = ClassKind::Tabbing; break;
            case 'n': result.classKind = ClassKind::Text; break;
            case 'r': result.defKind = DefKind::Reference; break;
            case 'c': result.defKind = DefKind::Citation; break;
            case 'C': result.defKind = DefKind::Citation; break; // TODO
            case 'l': result.defKind = DefKind::Label; break;
            case 'd': result.defKind = DefKind::Definition; break;
            case 'g': result.defKind = DefKind::Graphics; break;
            case 'i': result.defKind = DefKind::File; break;
            case 'u': result.defKind = DefKind::Package; break;
            case 'b': result.defKind = DefKind::Bibliography; break;
            case 'U': result.defKind = DefKind::Url; break;
            case 'K': result.defKind = DefKind::Bracketlike; break;
            case 'D': result.defKind = DefKind::Todo; break;
            case 'B': result.defKind = DefKind::Color; break;
            case 's': result.defKind = DefKind::Special; break;
            case 'V': result.defKind = DefKind::Verbatim; break;
            case 'N': result.defKind = DefKind::Newtheorem; break;
            case 'L':
                result.defKind = DefKind::Structure;
                extractStructureLevel(result, classification, ++i);
                break;
            case '/':
                extractValidEnvs(result, classification, ++i);
                break;
            case '\\':
                extractLikeEnv(result, classification, ++i);
                break;
            default: {
                return result; // skip unknown
            }
        }
    }

    return result;
}

void processSnippet (string &prefix, string &command, string &meta, CWLDef &data, bool includeOptional, bool includePlaceholders) {
    Classification classification = identifyClassfification(meta);

    if (!classification.visible) { return; }

    string snippet = extractSnippet(command, includeOptional, includePlaceholders);

    data.snippets[CWL::EnvKind::General].emplace_back(CWL::CWLSnippet {
        prefix, snippet, CWL::EnvKind::General
    });
}

void processCommand (string &line, CWLDef &data, bool includeOptional, bool includePlaceholders) {
    assert(line[0] == '\\');

    string command {};
    string classification {};

    auto splitPoint = line.find('#');

    if (splitPoint == string::npos) {
        command = line;
    } else {
        command = line.substr(0, splitPoint);
        classification = line.substr(splitPoint + 1, line.size() - splitPoint - 1);
    }

    string prefix = command.substr(1, command.size() - 1);
    string snippet {};

    for (size_t i = 1; i < command.size(); i++) {
        if (!isalpha(command[i]) && command[i] != '*') {
            prefix = command.substr(1, i - 1);
            break;
        }
    }

    if (prefix == "begin") {
        processEnvironment(command, classification, data);
    } else {
        processSnippet(prefix, command, classification, data, includeOptional, includePlaceholders);
    }
}

void processCommands (CWLDef &data, vector<string> &commands) {
    bool includeOptional = g_config->latex.cwl.includeOptional;
    bool includePlaceholders = g_config->latex.cwl.includePlaceholders;

    for (auto &command : commands) {
        processCommand(command, data, includeOptional, includePlaceholders);
    }
}

CWLDef parseCWL (const string &path) {
    CWLDef data {};
    auto lines = getFileContents(path);
    auto commands = extractCommands(lines);
    processCommands(data, commands);
    return data;
}

string locateCWLDirectory () {
    string HOME = std::getenv("HOME"); // TODO: Windows support
    return HOME + "/.config/latex-language-server/cwl";
}

void locateAndBuildCWLFiles () {
    fs::path directory ( locateCWLDirectory() );

    if (!fs::exists(directory)) {
        return;
    }

    vector<string> cwlFilePaths {};
    for (auto &p : fs::directory_iterator(directory)) {
        if (p.path().extension() == ".cwl") {
            cwlFilePaths.emplace_back(p.path());
        }
    }

    for (auto &file : cwlFilePaths) {
        auto cwlData = parseCWL(file);
        string package = fs::path(file).filename();
        package.erase(package.size() - 4); // remove extension
        g_config->latex.cwl.cwlFiles.insert({package, cwlData});
    }

    g_config->latex.cwl.initialised = true;
}

unordered_map<string, CWLDef> *getCWLFiles () {
    if (!g_config->latex.cwl.initialised) {
        locateAndBuildCWLFiles();
    }

    return &g_config->latex.cwl.cwlFiles;
}
