#ifndef LATEX_LANGUAGE_SERVER_FILE_H
#define LATEX_LANGUAGE_SERVER_FILE_H

#include <iostream>
#include <tree_sitter/runtime.h>

#include <text-buffer.h>

#include "Uri.h"
#include "UtfHandler.h"

class File {
private:
    Uri uri;

    string languageId;

    size_t version { 0 };

    TSParser *parser { nullptr };

    TextBuffer buffer {};

    UtfHandler utf {};

public:
    File (Uri &uri, string &languageId, string &text);

    // All possibly relevant extentions
    enum class Type {
        Tex,
        Log,
        Aux,
        Bib,
        Bbl,
        Blg,
        Bst,
        Other // support arbitrary files if necessary (using language ID)
    };

    bool operator == (File &that);

    void print (std::ostream &stream);
};

#endif //LATEX_LANGUAGE_SERVER_FILE_H
