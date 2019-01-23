#ifndef LATEX_LANGUAGE_SERVER_BIBINDEXER_H
#define LATEX_LANGUAGE_SERVER_BIBINDEXER_H

#include <string>
#include <vector>
#include <filesystem/File.h>

using std::u16string;
using std::vector;

enum class Severity {
    Error = 1,
    Warning = 2,
    Info = 3,
    Hint = 4
};

enum Error {
    Generic
};

struct BibEntryIssue : Init::Reflectable {
    void reflect (StringWriter &writer) override;

    BibEntryIssue (TSRange range, Severity sev, Error code, string msg) : range { range }, severity { sev }, code { code }, message { msg } {}

    // generic issue that can be translated to a linter message
    TSRange range;
    Severity severity;
    Error code;
    string message; // TODO: remove from here and generate on demand based on code?
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

public:
    BibIndexer () = default;

    explicit BibIndexer (File *file) : file (file) {};

    void completeIndex ();

    void partialReindex (Point &changesStart);

    void clearCached ();

    void addError (const TSNode &node, Error error, const string &msg);

    void lintErrors (TSNode rootNode);

    void publishErrors ();
};

#endif //LATEX_LANGUAGE_SERVER_BIBINDEXER_H
