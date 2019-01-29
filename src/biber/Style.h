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

enum class Type {
    String,
    List
};

// TODO: Use vector of u16string when `List` type
struct Constant {
    enum class Type {
        String,
        List
    } type { Type::String };

    u16string value {};
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
    vector<u16string> fields {};
    Field::ConstraintType type { Field::ConstraintType::None };
    int rangeMin { 0 };
    int rangeMax { 0 };
    string pattern {}; // TODO: make regex object
};

struct MandatoryConstraint {
    vector<u16string> all {};
    vector<vector<u16string>> some {};
    vector<vector<u16string>> one {};
};

struct TempConstraintData {
    vector<u16string> entries {};
    vector<MandatoryConstraint> mandatory {};
    vector<ConditionalConstraint> conditional {};
    vector<DataConstraint> data {};
};

std::ostream & operator << (std::ostream &out, const Field &field);

struct Entry {
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
    UtfHandler utf {};

    string name {};

    unordered_map<u16string, Constant> constants;

    unordered_map<u16string, Entry> entries;

    void parseDatamodel (xml_node<> *datamodel, vector<TempConstraintData> & tempConstraints, vector<u16string> &universalFields, unordered_map<u16string, vector<u16string>> &entryFields, unordered_map<u16string, Field> &fields);

    void parseConstants (xml_node<> *def);

    void parseEntryTypes (xml_node<> *def);

    void parseFields (xml_node<> *def, unordered_map<u16string, Field> &fields);

    void parseEntryFields (xml_node<> *def, vector<u16string> &universalFields, unordered_map<u16string, vector<u16string>> &entryFields);

    void parseConstraints (xml_node<> *def, vector<TempConstraintData> &tempConstraints);

public:
    explicit Style (xml_document<> &doc);

    optional<Entry> getEntry (u16string &name);

    optional<Field> getField (u16string &name);
};

} // namespace Style

#endif //LATEX_LANGUAGE_SERVER_STYLE_H
