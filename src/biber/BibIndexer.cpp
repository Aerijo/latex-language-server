#include <tree_sitter/parser.h>
#include "BibIndexer.h"

#include <stack>
#include <lsp-tools/messaging.h>

extern "C" {
    TSLanguage *tree_sitter_biber();
}

using std::stack;

void makeLowerCase (u16string &input) {
    // TODO: Full unicode support (but ASCII should be adequate for bibtex compliant sources)
    auto end = input.end();
    for (auto itr = input.begin(); itr != end; itr++) {
        auto &c = *itr;

        if ('A' <= c && c <= 'Z') {
            c += 32;
        }
    }
}

void BibEntryIssue::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("range"); ADD_TS_RANGE(range);
    writer.Key("severity"); writer.Int(static_cast<int>(severity));
    writer.Key("code"); writer.Int((int) code);
    writer.Key("source"); writer.String(source);
    writer.Key("message"); writer.String(message);
    writer.EndObject();
}

void BibIndexer::addLint (const TSNode &node, Severity sev, int code, const string &msg) {
    TSPoint start = ts_node_start_point(node);
    TSPoint end = ts_node_end_point(node);
    issues.emplace_back(BibEntryIssue { { start, end }, sev, code, msg });
}

void BibIndexer::addError (const TSNode &node, Error error, const string &msg) {
    addLint(node, Severity::Error, (int) error, msg);
}

void BibIndexer::addWarning (const TSNode &node, Warning warning, const string &msg) {
    addLint(node, Severity::Warning, (int) warning, msg);
}

void BibIndexer::addInfo (const TSNode &node, Info info, const string &msg) {
    addLint(node, Severity::Info, (int) info, msg);
}

void BibIndexer::lintErrors (TSNode rootNode) {
    stack<TSNode> path;

    path.push(rootNode);

    while (!path.empty()) {
        const TSNode node = path.top(); path.pop();

        if (ts_node_has_error(node)) {
            TSSymbol symbol = ts_node_symbol(node);
            if (symbol == ts_builtin_sym_error) {
                addError(node, Error::Generic, "Error!");
            } else {
                uint32_t numChildren = ts_node_child_count(node);
                for (uint32_t i = 0; i < numChildren; i++) {
                    TSNode child = ts_node_child(node, i);
                    path.push(child);
                }
            }
        }
    }
}

void BibIndexer::completeIndex () {
    clearCached();
    assert (file != nullptr);

    const TSTree *tree = file->getParseTree();
    assert (tree != nullptr);

    std::cerr << "reindexing bib file " << file->getPath() << "\n";

    TSNode rootNode = ts_tree_root_node(tree);

    lintErrors(rootNode);

    lintFile(rootNode);

    std::cerr << "found " << issues.size() << " issues...\n";

    publishErrors();

    std::cerr << "published errors\n";
}

void BibIndexer::partialReindex (Point &changesStart) {

}

void BibIndexer::clearCached () {
    entries.clear();
    strings.clear();
    issues.clear();
    keys.clear();
}

void BibIndexer::publishErrors () {
    INIT_WRITER

    writer.Key("method"); writer.String("textDocument/publishDiagnostics");

    writer.Key("params"); writer.StartObject();
        writer.Key("uri"); writer.String(file->getPath());
        writer.Key("diagnostics"); writer.StartArray();
            for (auto issue : issues) {
                issue.reflect(writer);
            }
        writer.EndArray();
    writer.EndObject();

    SEND_MESSAGE
}

// Copied from tree-sitter-biber/src/parser.c
// TODO: Automate this somehow (needs to be compile time constant for switch statements)
enum Sym {
    SymComment = 1,
    SymIdentifier = 16,
    SymInteger = 18,
    SymJunk = 25,
    SymEntry = 30,
    SymKey = 31,
    SymName = 32,
    SymField = 33,
    SymValue = 34,
    SymString = 36
};

void BibIndexer::lintFile (TSNode rootNode) {
    std::cerr << "Fully linting bib file " << file->getPath() << "\n";

    if (style == nullptr) {
        std::cerr << "Cannot analyse bib file without style!\n"; // though can do keys and such...
    }

    auto num_children = ts_node_child_count(rootNode);
    for (uint32_t i = 0; i < num_children; i++) {
        TSNode child = ts_node_child(rootNode, i);

        switch (ts_node_symbol(child)) {
            case SymEntry:
                lintEntry(child);
            default: {}
        }
    }
}

bool BibIndexer::getEntryName (u16string &entryName, TSNode &entryNode, uint32_t &index, uint32_t childCount) {
    for (; index < childCount; index++) {
        TSNode node = ts_node_named_child(entryNode, index);

        switch (ts_node_symbol(node)) {
            case SymName:
                entryName = file->textForNode(node);
                makeLowerCase(entryName);
                return true;
            case SymComment:
                // TODO: Complain about comments inside the entry block
                break;
            default:
                addError(entryNode, Error::Unexpected, "Unexpected value");
                return false;
        }
    }

    return false;
}

bool BibIndexer::getEntryKey (u16string &keyName, TSNode &entryNode, uint32_t &index, uint32_t childCount) {
    for (; index < childCount; index++) {
        TSNode node = ts_node_named_child(entryNode, index);
        switch (ts_node_symbol(node)) {
            case SymKey:
                keyName = file->textForNode(node);
                return true;
            default: {}
        }
    }

    addError(entryNode, Error::MissingKeyName, "Missign key name");
    return false;
}

bool hasNonASCIIchar (u16string &input) {
    for (auto &c : input) {
        if (c > 122) return true;
    }
    return false;
}

void BibIndexer::lintEntry (TSNode &entryNode) {
    // TODO: Work out exactly how much gets linted
    //   when missing things such as entry name, key, etc
    //   (currently we give up as soon as one doesn't exist)

    u16string entryName {};

    vector<std::pair<u16string, TSNode>> observedFields {}; // we're likely to have at most 10, so a vector should be better than a map

    uint32_t childCount = ts_node_named_child_count(entryNode);
    uint32_t i = 0;

    bool foundEntryName = getEntryName(entryName, entryNode, i, childCount);
    if (!foundEntryName) return;

    if (entryName.empty()) {
        TSNode entryNameNode = ts_node_child(entryNode, 0);
        addError(entryNameNode, Error::MissingEntryName, "No entry name");
        return;
    }

    std::optional<Style::Entry *> oentry = style->getEntry(entryName);

    TSNode entryNameNode = ts_node_named_child(entryNode, i);

    if (!oentry) {
        addWarning(entryNameNode, Warning::UnknownEntry, "Entry " + utf.utf16to8(entryName) + " is unexpected");
        return;
    }

    Style::Entry &entry = *oentry.value();

    u16string entryKey;
    bool foundEntryKey = getEntryKey(entryKey, entryNode, i, childCount);
    if (!foundEntryKey) return;

    TSNode keyNode = ts_node_named_child(entryNode, i);

    if (entryKey.empty()) {
        addWarning(entryNameNode, Warning::EmptyKey, "Key is empty");
    } else {
        if (keys.count(entryKey) > 0) {
            addWarning(keyNode, Warning::DuplicateKey, "Duplicate key name");
        } else {
            keys.insert({ entryKey });
        }
    }

    if (hasNonASCIIchar(entryKey)) {
        addWarning(keyNode, Warning::NonASCIIKey, "Non ASCII characters detected in key");
    }

    lintFields(entryNode, observedFields, i, childCount);

    if (oentry) {
        postProcessEntry(entryNameNode, entry, observedFields);
    }
}

void BibIndexer::lintFields (TSNode &entry, vector<std::pair<u16string, TSNode>> &observedFields, uint32_t &index, uint32_t childCount) {
    for (; index < childCount; index++) {
        TSNode node = ts_node_named_child(entry, index);

        switch (ts_node_symbol(node)) {
            case SymField:
                lintField(node, observedFields);
            default: {}
        }
    }
}

void BibIndexer::lintField (TSNode &fieldNode, vector<std::pair<u16string, TSNode>> &observedFields) {
    uint32_t childCount = ts_node_named_child_count(fieldNode);

    TSNode identifierNode {};
    uint32_t i = 0;

    bool foundIdentifier { false };

    for (; i < childCount; i++) {
        TSNode node = ts_node_named_child(fieldNode, i);
        if (ts_node_symbol(node) == SymIdentifier) {
            identifierNode = node;
            foundIdentifier = true;
            break;
        }
    }

    if (!foundIdentifier) {
        addError(fieldNode, Error::MissingFieldName, "Missing field identifier");
        return;
    }

    u16string fieldName = file->textForNode(identifierNode);
    makeLowerCase(fieldName);

    if (fieldName.empty()) {
        addError(fieldNode, Error::MissingFieldName, "Empty field identifier");
        return;
    }

    for (auto &pair : observedFields) {
        if (pair.first == fieldName) {
            addWarning(identifierNode, Warning::DuplicateField, "Duplicates existing field in entry");
            return;
        }
    }

    observedFields.emplace_back(std::pair { fieldName, identifierNode });

    // we should check expectedness in post processing, as it's possible a map
    // converts this field to another (not that we support maps yet mind)
}

void BibIndexer::postProcessEntry (const TSNode &entryNameNode, const Style::Entry &entryStyle, vector<std::pair<u16string, TSNode>> &observedFields) {
    // apply the maps somewhere here

    auto expectedFields = entryStyle.fields;

    // first check if all are expected
    for (const auto &pair : observedFields) {
        const u16string &fieldName = pair.first;
        const TSNode &fieldNameNode = pair.second;

        if (fieldName == u"date") {
            // TODO: Match Biber's special date handling
        } else {
            const auto itr = expectedFields.find(fieldName);
            if (itr == expectedFields.end()) {
                addWarning(fieldNameNode, Warning::UnexpectedField, "Unexpected field " + utf.utf16to8(fieldName));
                continue;
            }

            Style::Field &fieldStyle = itr->second;
            optional<u16string> fieldValue = resolveFieldValue(fieldNameNode);

            if (!fieldStyle.nullok && fieldValue && fieldValue->empty()) {
                addWarning(fieldNameNode, Warning::EmptyField, "Empty field value not permitted");
            }
        }
    }

    checkEntryConstraints(entryNameNode, entryStyle, observedFields);
}

optional<u16string> BibIndexer::resolveFieldValue (const TSNode &fieldNameNode) {
    // TODO: Account for maps, string constants, concatenation, etc
    TSNode node = ts_node_next_named_sibling(fieldNameNode);

    for (; !ts_node_is_null(node); node = ts_node_next_named_sibling(node)) {
        if (ts_node_symbol(node) == SymValue) {
            if (ts_node_child_count(node) != 1) return {};
            TSNode value = ts_node_child(node, 0);

            switch (ts_node_symbol(value)) {
                case SymInteger:
                    return file->textForNode(value);
                case SymString:
                    if (ts_node_named_child_count(value) == 1) { // "foo" <-- string, foo <-- text
                        return file->textForNode(ts_node_named_child(value, 0));
                    } else {
                        return optional{ u16string {} };
                    }
                default:
                    return {};
            }
        };
    }

    return {};
}

void BibIndexer::checkEntryConstraints (const TSNode &entryNameNode, const Style::Entry &entryStyle, vector<std::pair<u16string, TSNode>> &observedFields) {
    vector<u16string> missingAll {};
    vector<vector<u16string>> missingSome {};
    vector<vector<u16string>> missingOne {};
    vector<vector<u16string>> extraOne {};

    auto &constraints = entryStyle.constraints;
    for (const auto &field : constraints.all) {
        bool seen { false };

        for (const auto &pair : observedFields) {
            if (pair.first == field) {
                seen = true;
                break;
            }
        }

        if (!seen) {
            missingAll.emplace_back(field);
        }
    }

    for (const auto &fields : constraints.some) {
        bool seen { false };

        for (const auto &field : fields) {
            for (const auto &pair : observedFields) {
                if (pair.first == field) {
                    seen = true;
                    break;
                }
            }

            if (seen) break;
        }

        if (!seen) {
            missingSome.emplace_back(fields);
        }
    }

    for (const auto &fields : constraints.one) {
        int seen { 0 };

        for (const auto &field : fields) {
            for (const auto &pair : observedFields) {
                if (pair.first == field) {
                    seen++;
                    break;
                }
            }

            if (seen > 1) break;
        }

        if (seen == 0) {
            missingOne.emplace_back(fields);
        } else if (seen > 1) {
            extraOne.emplace_back(fields);
        }
    }

    if (!missingAll.empty()) {
        string fields { "[" };
        for (auto &field : missingAll) {
            fields.append(utf.utf16to8(field));
            fields.append(", ");
        }
        fields.resize(fields.size() - 2);
        fields.append("]");
        addWarning(entryNameNode, Warning::MissingField, "Missing fields " + fields);
    }

    for (auto &someFields : missingSome) {
        string fields { "[" };
        for (auto &field : someFields) {
            fields.append(utf.utf16to8(field));
            fields.append(", ");
        }
        fields.resize(fields.size() - 2);
        fields.append("]");
        addWarning(entryNameNode, Warning::MissingSomeField, "Missing at least one field of " + fields);
    }

    for (auto &oneFields : missingOne) {
        string fields { "[" };
        for (auto &field : oneFields) {
            fields.append(utf.utf16to8(field));
            fields.append(", ");
        }
        fields.resize(fields.size() - 2);
        fields.append("]");
        addWarning(entryNameNode, Warning::MissingOneField, "Missing one field of " + fields);
    }

    for (auto &extraFields : extraOne) {
        string fields { "[" };
        for (auto &field : extraFields) {
            fields.append(utf.utf16to8(field));
            fields.append(", ");
        }
        fields.resize(fields.size() - 2);
        fields.append("]");
        addWarning(entryNameNode, Warning::TooManyField, "To many fields of " + fields);
    }
}
