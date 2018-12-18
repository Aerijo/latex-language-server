//
// Created by Benjamin Gray on 17/12/18.
//

#include "options.h"

map<string, string> parse_options (int argc, char** argv) {
    map<string, string> options;

    for (int i = 1; i < argc; i++) {
        options[std::to_string(i)] = argv[i];
    }

    return options;
}