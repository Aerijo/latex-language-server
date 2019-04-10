#ifndef LATEX_LANGUAGE_SERVER_FILE_H
#define LATEX_LANGUAGE_SERVER_FILE_H

#include <iostream>
#include <tree_sitter/runtime.h>

#include <text-buffer.h>

#include "lsp-tools/definitions.h"
#include "Uri.h"
#include "UtfHandler.h"

#include <mutex>

/**
 * A logical reflection of a file on the user's computer, including changes in the editor
 * Kept in sync via JSON RPC document events
 */
class File {
private:
    Uri::Uri uri;

    string languageId;

    versionNum version { 0 };

    TSParser *parser { nullptr };

    TSTree *tree { nullptr };

    TextBuffer buffer {};

    UtfHandler utf {};

    bool openInEditor { true };

    std::mutex mtx;

public:
    bool hasParser { false };

    File () = delete;

    File (Uri::Uri &uri, string &languageId, string &text);

    File (string &uri, string &languageId, versionNum version, string &text);

    ~File ();

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
    } type;

    bool operator == (File &that);

    void print (std::ostream &stream);

    void setTextInRange (Range &oldRange, std::string &&text);

    void setText (std::string &&text);

    void setTextInRange (Range &oldRange, std::u16string &&text);

    u16string textForNode (const TSNode &node);

    string utf8TextForNode (const TSNode &node);

    u16string textInRange (const Range &range);

    string utf8TextInRange (const Range &range);

    u16string textForRow (uint32_t row);

    string utf8textForRow (uint32_t row);

    Point getEndPoint ();

    bool validNextVersion (versionNum nextVersion);

    void setVersion (versionNum nextVersion);

    void executeParse ();

    void setupParser ();

    TSTree *getParseTree ();

    TSNode getRootNode ();

    string getPath ();

    bool isBibtex () {
        return languageId == "bibtex" || languageId == "biber";
    }
};

#endif //LATEX_LANGUAGE_SERVER_FILE_H
