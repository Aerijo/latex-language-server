//
// Created by Benjamin Gray on 17/12/18.
//

#ifndef LATEX_LANGUAGE_SERVER_OPTIONS_H
#define LATEX_LANGUAGE_SERVER_OPTIONS_H

#include <map>
#include <string>

using std::map;
using std::string;

map<string, string> parse_options (int argc, char** argv);

#endif //LATEX_LANGUAGE_SERVER_OPTIONS_H
