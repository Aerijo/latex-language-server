#ifndef LATEX_LANGUAGE_SERVER_FILE_H
#define LATEX_LANGUAGE_SERVER_FILE_H

#include <iostream>
#include <tree_sitter/runtime.h>

#include <text-buffer.h>

#include "Uri.h"
#include "UtfHandler.h"

/**
 * A logical reflection of a file on the user's computer, including changes in the editor
 * Kept in sync via JSON RPC document events
 */
class File {
private:
    Uri uri;

    string languageId;

    uint_fast64_t version { 0 };

    TSParser *parser { nullptr };

    TextBuffer buffer {};

    UtfHandler utf {};

    bool openInEditor { true };

public:
    File (Uri &uri, string &languageId, string &text);
    File (string &uri, string &languageId, string &text);

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

    void setTextInRange(Range oldRange, std::u16string &&text);
};

#endif //LATEX_LANGUAGE_SERVER_FILE_H
