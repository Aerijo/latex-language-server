#ifndef LATEX_LANGUAGE_SERVER_STYLE_H
#define LATEX_LANGUAGE_SERVER_STYLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <optional.h>
#include <rapidxml.hpp>
#include <vector>
#include <optional>
#include <filesystem/UtfHandler.h>

/*
 * Used to store all the expected entries, fields, field values, etc.
 *
 * .bcf schema: https://github.com/plk/biber/blob/dev/data/schemata/bcf.rnc
 *
 * PLANNED VALIDATION PROCESS:
 * For every entry:
 *  - Read it's name & fields
 *  - Apply any maps (e.g., @phdthesis -> @thesis{type="phd"})
 *  - Check contraints
 */

using std::unordered_set;
using std::string;
using std::vector;
using std::u16string;
using std::unordered_map;

using namespace rapidxml;

namespace Style {

// TODO: Use vector of u16string when `List` type
struct Constant {
    enum class Type {
        String,
        List
    } type { Type::String };

    u16string value {};
};

struct MapStep {
    // Only a small number (compared to what's possible) will be handled initially. This can be improved upon.
    enum class Type {
        Unhandled,
        RenameEntry,
        RenameField,
        SetField,
    } type { Type::Unhandled };

    u16string typeSource {};
    u16string typeTarget {};
    bool final { false };

    u16string fieldSet {};
    u16string fieldValue {};
    u16string fieldSource {};
    u16string fieldTarget {};
};

struct SourceMap { // a collation of map_step's (inside a map)
    vector<u16string> forEntries {};
    unordered_set<u16string> notForEntries {};

    vector<std::pair<u16string, u16string>> entryAliases {}; // phdthesis -> thesis
    vector<std::pair<u16string, u16string>> fieldAliases {}; // journal -> journaltitle

    unordered_map<u16string, vector<std::pair<u16string, u16string>>> entryFieldAliases {};

    vector<u16string> setFields {}; // day is set globally apparently (to null)
    unordered_map<u16string, vector<u16string>> setEntryFields {}; // phdthesis sets type
};

struct ConditionalConstraint {
    enum class Quant {
        All,
        One,
        None
    };

    struct {
        Quant condition { Quant::All };
        vector<u16string> fields {};
    } pre {};

    struct {
        Quant condition { Quant::All };
        vector<u16string> fields {};
    } post {};
};

struct Field {
    Field () = delete;

    explicit Field (u16string &name) : name { name } {};

    u16string name {};

    enum class Type {
        Field,
        List
    } type { Type::Field };

    enum class Format {
        Unspecified,
        XSV,
        Other,
    } format { Format::Unspecified };

    enum class Data {
        Literal,
        Name,
        Key,
        EntryKey,
        Date,
        Verbatim,
        Integer,
        Range,
        Code,
        URI,
        Datepart,
        Keyword,
        Option,
    } dataType { Data::Literal };

    bool nullok { false };
    bool skipout { false };
    bool label { false };

    enum class ConstraintType {
        None,
        Integer,
        Isbn,
        Issn,
        Ismn,
        Datepart,
        Pattern
    } constraintType { ConstraintType::None };

    int rangeMax {};
    int rangeMin {};
    string pattern; // TODO: make regex object
};

struct DataConstraint {
    unordered_set<u16string> fields {};
    Field::ConstraintType type { Field::ConstraintType::None };
    int rangeMin { 0 };
    int rangeMax { 0 };
    string pattern {}; // TODO: make regex object
};

struct MandatoryConstraint {
    unordered_set<u16string> all {};
    vector<vector<u16string>> some {};
    vector<vector<u16string>> one {};
};

struct TempConstraintData { // one of these per "bcf:constraints". Each can hold multiple "bcf:constraint"
    unordered_set<u16string> entries {};
    MandatoryConstraint mandatory {}; // Technically can have multiple inside a single "bcf:constraints", but are equivalent to a single one when merged
    vector<ConditionalConstraint> conditional {};
    vector<DataConstraint> data {};
};

std::ostream & operator << (std::ostream &out, const Field &field);

struct Entry {
    Entry () = delete;

    Entry copy ();

    explicit Entry (u16string &name, bool skipOut = false) : name { name }, skipOut { skipOut } {}

    u16string name {};
    unordered_map<u16string, Field> fields {};
    bool skipOut { false };

    struct Constraints {
        vector<u16string> all {};
        vector<vector<u16string>> some {};
        vector<vector<u16string>> one {};
        vector<ConditionalConstraint> conditional {};
    } constraints;
};

class Style {
private:
    bool loaded { false };

    UtfHandler utf {};

    string name {};

    unordered_map<u16string, Constant> constants;

    unordered_map<u16string, Entry> entries;

    void buildFromXML (xml_document<> &doc);

    void parseDatamodel (xml_node<> *datamodel, vector<TempConstraintData> & tempConstraints, vector<u16string> &universalFields, unordered_map<u16string, vector<u16string>> &entryFields, unordered_map<u16string, Field> &fields);

    void parseConstants (xml_node<> *def);

    void parseSourcemap (xml_node<> *def, vector<SourceMap> &sourcemaps);

    void parseMaps (xml_node<> *def, vector<SourceMap> &maps);

    void parseEntryTypes (xml_node<> *def);

    void parseFields (xml_node<> *def, unordered_map<u16string, Field> &fields);

    void parseEntryFields (xml_node<> *def, vector<u16string> &universalFields, unordered_map<u16string, vector<u16string>> &entryFields);

    void parseConstraints (xml_node<> *def, vector<TempConstraintData> &tempConstraints);

    void buildEntryFields (vector<u16string> &universalFields, unordered_map<u16string, vector<u16string>> &entryFields, unordered_map<u16string, Field> &fields);

    void addEntryConstraints (vector<TempConstraintData> &tempConstraints);

    void applySourcemaps (vector<SourceMap> sourcemaps);

    void consolidateEntryConstraints ();

public:
    Style () = default;

    explicit Style (const string &path);

    explicit Style (xml_document<> &doc);

    std::optional<Entry *> getEntry (const u16string &name);
};

std::ostream &operator << (std::ostream &out, const Field &field);

std::ostream &operator << (std::ostream &out, const u16string &input);

template<typename T>
std::ostream &operator << (std::ostream &out, const vector<T> &input) {
    out << "[";
    for (const auto &entry : input) {
        out << entry << ", ";
    }
    if (!input.empty()) out << "\b\b";
    out << "]";
    return out;
};

template <typename K, typename V>
std::ostream &operator << (std::ostream &out, const unordered_map<K, V> &input) {
    out << "<";
    for (auto &pair : input) {
        out << pair.first << ": " << pair.second << ", ";
    }
    if (!input.empty()) out << "\b\b";
    out << ">";
    return out;
}

std::ostream &operator << (std::ostream &out, const Entry &input);

} // namespace Style

#endif //LATEX_LANGUAGE_SERVER_STYLE_H
