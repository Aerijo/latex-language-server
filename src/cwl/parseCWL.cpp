#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using std::string;
using std::vector;
using std::unordered_map;

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

void extractArgument (string &snippet, string &line, size_t &i, int &placeNum) {
    auto start = i + 1;
    for (; i < line.size(); i++) {
        if (line[i] == '}') { break; }
    }
    string argument = line.substr(start, i - start);
    snippet += "{${" + std::to_string(++placeNum) + ":" + argument + "}}";
}

void extractOptionalArgument (string &snippet, string &line, size_t &i, int &placeNum) {
    auto start = i + 1;
    for (; i < line.size(); i++) {
        if (line[i] == ']') { break; }
    }
    string argument = line.substr(start, i - start);
    snippet += "[${" + std::to_string(++placeNum) + ":" + argument + "}]";
}

string convertToSnippet (string &line) {
    size_t i = 1;
    for (; i < line.size(); i++) {
        if (!isalpha(line[i])) { break; }
    }

    string commandName = "\\" + line.substr(0, i);

    string snippet = "\\\\" + commandName;

    int placeNum = 0;

    for (; i < line.size(); i++) {
        switch (line[i]) {
            case '{':
                extractArgument(snippet, line, i, placeNum);
                break;
            case '[':
                extractOptionalArgument(snippet, line, i, placeNum);
                break;
            case '%':
                // cursor placement
                break;
            case '#':
                // end of definition (metadata follows)
                break;
            case '\\':
                snippet += R"|(\\\\)|";
                break;
            default:
                snippet += line[i];
        }
    }


    return snippet;
}

vector<string> extractSnippets (vector<string> &commands) {
    vector<string> snippets (commands.size());
    for (size_t i = 0; i < commands.size(); i++) {
        snippets[i] = convertToSnippet(commands[i]);
    }
    return snippets;
}

void parseCWL (const string &path) {
    auto lines = getFileContents(path);
    auto commands = extractCommands(lines);
    auto snippets = extractSnippets(commands);

    for (auto &line : snippets) {
        std::cout << line << "\n";
    }
}
