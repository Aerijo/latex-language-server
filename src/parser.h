#ifndef LATEX_PARSER
#define LATEX_PARSER

#include<map>
#include<string>

using std::map;
using std::string;


struct LatexFileData {
    // TODO: Recognise commands in mathmode vs plain text
    // NOTE: This requires group tracking
    map<string, string> magic_comments;
    map<string, int> commands;
    map<string, int> environments;

    string path;


    explicit LatexFileData (const string &path) {
        this->path = path;
    }
};


LatexFileData parse_latex_text (string text);

void print_file_data (LatexFileData data);

#endif
