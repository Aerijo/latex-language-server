#include "./parser.h"

#include<fstream>
#include<iostream>
#include<experimental/filesystem>

#define SKIP_WHITESPACE(stream) while (stream.peek() == ' ') stream.get();
#define TRIM_END(str) str = str.substr(0, str.find_last_not_of(' ') + 1);

using namespace std;

enum Mode {
    READ,
    COMMENT,
    COMMAND,
    ENV_NAME
};

void goto_end_of_line(ifstream &file) {
    file.ignore(numeric_limits<streamsize>::max(), '\n');
}


void get_comment (ifstream &file, map<string, string> &magic_comments) {
    SKIP_WHITESPACE(file)

    if (file.get() != '!') return goto_end_of_line(file);
    if (file.get() != 'T') return goto_end_of_line(file);
    if (file.peek() != 'e' && file.peek() != 'E') return goto_end_of_line(file);
    file.get();
    if (file.get() != 'X') return goto_end_of_line(file);
    if (file.get() != ' ') return goto_end_of_line(file);

    SKIP_WHITESPACE(file)

    char c;
    string key;

    while (file.get(c)) {
        if (!isalnum(c) && c != '_' && c != '-') {
            if (c == ' ' || c == '=') break;
            return goto_end_of_line(file);
        }
        key += c;
    }

    SKIP_WHITESPACE(file)

    if (file.get() != '=') return goto_end_of_line(file);

    SKIP_WHITESPACE(file)

    string value;
    while (file.get(c) && c != '\n') {
        value += c;
    }

    TRIM_END(value);

    magic_comments[key] = value;
}

void get_environment (ifstream &file, LatexFileData &data) {
    SKIP_WHITESPACE(file)
    if (file.peek() != '{') return;

    string env_name;
}

LatexFileData parse_latex_text (const string path) {
    LatexFileData data (path);

    Mode mode = READ;

    string command_name;

    ifstream file (path);

    if (!file.is_open()) {
        cout << "Could not read file " << path << "\n";
        return data;
    }

    char c;
    while (file.get(c)) {
        switch (mode) {
            case COMMENT:
                get_comment(file, data.magic_comments);
                mode = READ;
                break;

            case READ:
                switch (c) {
                    case '%':
                        mode = COMMENT;
                        break;
                    case '\\':
                        mode = COMMAND;
                        break;
                }
                break;

            case COMMAND:
                if (isalpha(c)) {
                    command_name += c;
                    break;
                }

                if (command_name.length() == 0) {
                    command_name = c;
                } else if (command_name == "begin") {
                    mode = ENV_NAME;
                } else {
                    mode = READ;
                }

                data.commands[command_name]++;
                command_name = "";
                break;

            case ENV_NAME:
                get_environment(file, data);
                mode = READ;
                break;
        }
    }

    if (mode == COMMAND && command_name.length() > 0) {
        data.commands[command_name]++;
    }

    return data;
}

void print_file_data (LatexFileData data) {
    cout << "Logging file data for " << data.path << "\n";
    cout << "  Commands:\n";
    for (auto elem : data.commands)
    {
        std::cout << "    \"" << elem.first << "\": " << elem.second << "\n";
    }
    cout << "  Environments:\n";
    for (auto elem : data.environments)
    {
        std::cout << "    \"" << elem.first << "\": \"" << elem.second << "\"\n";
    }
    cout << "  Magic comments:\n";
    for (auto elem : data.magic_comments)
    {
        std::cout << "    \"" << elem.first << "\": \"" << elem.second << "\"\n";
    }
}
