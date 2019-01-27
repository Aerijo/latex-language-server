#ifndef LATEX_LANGUAGE_SERVER_STYLE_H
#define LATEX_LANGUAGE_SERVER_STYLE_H

#include <map>
#include <string>
#include <unordered_map>
#include <optional.h>
#include <rapidxml.hpp>
#include <vector>
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

using std::map;
using std::string;
using std::vector;
using std::u16string;
using std::unordered_map;

using namespace rapidxml;

namespace Style {

enum class ConstantType {
    String,
    List
};

// TODO: Use vector of u16string when `List` type
struct Constant {
    ConstantType type { ConstantType::String };
    u16string value {};
};

struct ConditionalConstraint {
};

enum class FieldType {
    Field,
    List,
    Other
};

enum class DataType {
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

    // These are not declared in the schema
    SeparatedValue,
    Pattern,
    Other
};

enum class Format {
    Unspecified,
    XSV,
    Other,
};

struct Field {
    explicit Field (u16string &name) : name { name } {};

    u16string name {};

    FieldType type { FieldType::Other };
    Format format { Format::Unspecified };
    DataType dataType { DataType::Other };

    bool nullok { false };
    bool skipout { false };
    bool label { false };
};

std::ostream & operator << (std::ostream &out, const Field &field);

struct Entry {
    explicit Entry (u16string &name, bool skipOut = false) : name { name }, skipOut { skipOut } {}

    u16string name {};
    unordered_map<u16string, Field *> fields {};
    bool skipOut { false };

    struct {
        vector<u16string> all {};
        vector<vector<u16string>> some {};
        vector<vector<u16string>> one {};
        vector<ConditionalConstraint> conditional {};
    } constraints;
};

class Style {
private:
    UtfHandler utf {};

    string name {};

    unordered_map<u16string, Constant> constants;

    unordered_map<u16string, Entry> entries;

    unordered_map<u16string, Field> fields;

    void parseDatamodel (xml_node<> *datamodel, vector<xml_node<> *> &, vector<u16string> &, map<u16string, vector<u16string>> &);

    void parseConstants (xml_node<> *def);

    void parseEntryTypes (xml_node<> *def);

    void parseFields (xml_node<> *def);

    void parseEntryFields (xml_node<> *def, vector<u16string> &universalFields, map<u16string, vector<u16string>> &entryFields);

    void parseConstraints (xml_node<> *def, vector<xml_node<> *> &constraints);

    void processEntries ();

    void processFields ();

    void processConstraints ();

public:
    explicit Style (xml_document<> &doc);

    optional<Entry> getEntry (u16string &name);

    optional<Field> getField (u16string &name);

    void addEntry (Entry &&entry);

    void addField (Field &&field);
};

} // namespace Style

#endif //LATEX_LANGUAGE_SERVER_STYLE_H
