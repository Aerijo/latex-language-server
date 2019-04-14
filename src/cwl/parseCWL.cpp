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

void processSnippet (string &prefix, string &command, string &classification, CWLDef &data, bool includeOptional, bool includePlaceholders) {
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
