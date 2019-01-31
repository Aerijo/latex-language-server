#ifndef LATEX_LANGUAGE_SERVER_BIBINDEXER_H
#define LATEX_LANGUAGE_SERVER_BIBINDEXER_H

#include <string>
#include <vector>
#include <filesystem/File.h>
#include "Style.h"

using std::u16string;
using std::vector;

enum class Severity {
    Error = 1,
    Warning = 2,
    Info = 3,
    Hint = 4
};

enum class Error {
    Generic,
    Unexpected,
    Entry,
    MissingEntryName,
    MissingKeyName
};

enum class Warning {
    UnknownEntry,
    DuplicateKey,
    EmptyKey,
    NonASCIIKey
};

enum class Info {

};

struct BibEntryIssue : Init::Reflectable {
    void reflect (StringWriter &writer) override;

    BibEntryIssue (TSRange range, Severity sev, int code, string msg) : range { range }, severity { sev }, code { code }, message { msg } {}

    // generic issue that can be translated to a linter message
    TSRange range;
    Severity severity;
    int code;
    string message; // TODO: remove from here and generate on demand based on error code?
    string source { "biber" };
};

/**
 * Holds all indexing information related to a given entry
 */
struct BibIndexEntry {
    u16string key;
    bool hasString; // refers to macros that expand based on previously defined @string's
    vector<BibEntryIssue> issues;
};

struct BibIndexString {
    u16string identifier;
    u16string value;
};


/**
 * Indexes a .bib file, providing diagnostic information
 * such as a vector of keys, unexpected entries / fields,
 * etc.
 *
 * To save serialisation space and general text syncing, we
 * will simply reparse the entire entry when a change intersects
 * any part of it (or at least any field). Values can be generated
 * on demand.
 */
class BibIndexer {
private:
    File *file { nullptr };

    vector<BibEntryIssue> issues;

    vector<BibIndexEntry> entries;

    vector<BibIndexString> strings;

    unordered_set<u16string> keys;

    Style::Style *style { nullptr };

public:
    BibIndexer () = delete;

    explicit BibIndexer (File *file, Style::Style *style) : file { file }, style { style } {};

    void completeIndex ();

    void partialReindex (Point &changesStart);

    void clearCached ();

    void addLint (const TSNode &node, Severity sev, int code, const string &msg);

    void addError (const TSNode &node, Error error, const string &msg);

    void addWarning (const TSNode &node, Warning warning, const string &msg);

    void addInfo (const TSNode &node, Info info, const string &msg);

    void publishErrors ();

    void lintErrors (TSNode rootNode);

    void lintFile (TSNode rootNode);

    void lintEntry (TSNode &entry);

    bool getEntryName (u16string &entryName, TSNode &node, uint32_t &index, uint32_t childCount);

    bool getEntryKey (u16string &entryName, TSNode &node, uint32_t &index, uint32_t childCount);

    void lintComment ();
};

#endif //LATEX_LANGUAGE_SERVER_BIBINDEXER_H
