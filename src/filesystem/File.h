#ifndef LATEX_LANGUAGE_SERVER_FILE_H
#define LATEX_LANGUAGE_SERVER_FILE_H

#include <tree_sitter/runtime.h>

#include "Uri.h"

class File {
    File (Uri &uri, string &languageId, string text) : uri (uri) {

    }

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

    Uri uri;

    string languageId;

    size_t version { 0 };

    TSParser *parser { nullptr };

    string text;

public:
    bool operator == (File &that);
};

#endif //LATEX_LANGUAGE_SERVER_FILE_H
