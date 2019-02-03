#include <iostream>
#include "Style.h"

#include <cstring>
#include <fstream>

#define CONSOLIDATE_CONSTRAINTS (true)

namespace {
    UtfHandler utf {};
}

namespace Style {

using std::strcmp;

/*
 * Generates a Style from a .bcf file (once I learn how it works -_-).
 *
 * The meaning of a .bcf file is described in the Biber docs,
 * which can be found most easily with `texdoc biber`.
 *
 * NOTE:
 *  - Initially only support a handful of hardcoded maps (the ones that appear in APA
 *      style citation should be good enough). Can later add arbitrary map parsing
 *      for things like hover (so it shows what the final field will look like).
 *  - The datamodel does not account for mapped from entries, like phdthesis. So we
 *      need to be careful about when and how we handle maps.
 *  - Date fields must end with "date" (fatal error!)
 *
 * TODO:
 *  - Add in config file support too (this file can extend the datamodel)
 *
 * (Handled) Types of Map:
 * - map_type_source -> map_type_target (entry rename; create pseudo entry in style)
 * - when the above matches, map_final="1", and map_field_set -> map_field_value (account for this by removing from constraints)
 * - map_field_source -> map_field_target
 */

#define IS(input) strcmp(name + 4, input) == 0 // all names start with `bcf:`
#define EXPECT_NAME(node, input) assert(strcmp(node->name(), input) == 0)
#define EXPECT_VAL(node, input) assert(strcmp(node->value(), input) == 0)

std::ostream &operator << (std::ostream &out, const Field &field) {
    out << utf.utf16to8(field.name);
    return out;
}

std::ostream &operator << (std::ostream &out, const u16string &input) {
    out << utf.utf16to8(input);
    return out;
}

std::ostream &operator << (std::ostream &out, const Entry &input) {
    out << ":" << input.name << " F=" << input.fields.size() << " C=" << input.constraints.all.size() << "-" << input.constraints.some.size() << "-" << input.constraints.one.size() <<  ":";
    return out;
}

Entry Entry::copy () {
    Entry copy { name, skipOut };

    copy.constraints = Entry::Constraints {
        constraints.all,
        constraints.some,
        constraints.one,
        constraints.conditional // TODO: Copy conditional constraints properly
    };

    copy.fields = fields;

    return copy;
}

void applyGlobalDataConstraints (unordered_map<u16string, Field> &fields, vector<TempConstraintData> &tempConstraints) {
    // TODO
}

void getDatamodelXML (const string &path, xml_document<> &doc) {
    std::ifstream infile { path };
    string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };

    char* cstr = new char[file_contents.size() + 1];  // Create char buffer to store string copy
    strcpy (cstr, file_contents.c_str());
    doc.parse<0>(cstr);
}

Style::Style (const string &path) {
    xml_document<> doc;
    getDatamodelXML(path, doc);
    buildFromXML(doc);
}

Style::Style (xml_document<> &doc) {
    buildFromXML(doc);
}

void Style::buildFromXML (xml_document<> &doc) {
    std::cerr << "constructing Style from xml...\n";
    EXPECT_NAME(doc.first_node(), "bcf:controlfile");
    // NOTE: bcf:options component="biber" contains encoding information.
    //  We will assume it's always UTF-8 for now.

    vector<TempConstraintData> tempConstraints {};
    vector<u16string> universalFields {};
    unordered_map<u16string, vector<u16string>> entryFields {};
    unordered_map<u16string, Field> fields {};

    vector<SourceMap> sourcemaps {};

    for (xml_node<> *node = doc.first_node()->first_node(); node; node = node->next_sibling()) {
        const char *name = node->name();
        if (IS("sourcemap")) {
            parseSourcemap(node, sourcemaps);
        } else if (IS("datamodel")) {
            parseDatamodel(node, tempConstraints, universalFields, entryFields, fields);
        }
    }

    applyGlobalDataConstraints(fields, tempConstraints);

    buildEntryFields(universalFields, entryFields, fields);

    addEntryConstraints(tempConstraints);

    if (CONSOLIDATE_CONSTRAINTS) consolidateEntryConstraints();

    applySourcemaps(sourcemaps);
}

void Style::parseSourcemap (xml_node<> *def, vector<SourceMap> &sourcemaps) {
    EXPECT_NAME(def, "bcf:sourcemap");
    // TODO: Handle level (user, style, driver) properly

    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        EXPECT_NAME(node, "bcf:maps");

        bool canParseMaps { false };

        for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            string attrVal = attr->value();
            if (attrName == "datatype") {
                if (attrVal == "bibtex") {
                    canParseMaps = true;
                } else {
                    // we don't have support for biblatexml parsing yet
                    break;
                }
            }
        }

        if (canParseMaps) {
            parseMaps(node, sourcemaps);
        }
    }
}

MapStep parseMapStep (xml_node<> *node) {
    EXPECT_NAME(node, "bcf:map_step");

    MapStep step { MapStep::Type::Unhandled };

    bool hasTSource { false };
    bool hasTTarget { false };
    bool hasFSet { false };
    bool hasFValue { false };
    bool hasFSource { false };
    bool hasFTarget { false };

    for (auto *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        string attrName = attr->name();
        u16string attrVal = utf.utf8to16(attr->value());

        if (attrName == "map_type_source") {
            hasTSource = true;
            step.typeSource = attrVal;
        } else if (attrName == "map_type_target") {
            hasTTarget = true;
            step.typeTarget = attrVal;
        } else if (attrName == "map_final") {
            step.final = true; // "1" is the only allowed value
        } else if (attrName == "map_field_set") {
            hasFSet = true;
            step.fieldSet = attrVal;
        } else if (attrName == "map_field_value") {
            hasFValue = true;
            step.fieldValue = attrVal;
        } else if (attrName == "map_field_source") {
            hasFSource = true;
            step.fieldSource = attrVal;
        } else if (attrName == "map_field_target") {
            hasFTarget = true;
            step.fieldTarget = attrVal;
        } else {
            return step; // TODO: handle other possible attributes
        }
    }

    if (hasTSource) {
        if (hasTTarget && !(hasFTarget || hasFValue || hasFSet || hasFSource)) {
            step.type = MapStep::Type::RenameEntry;
        }
    } else if (hasFSource) {
        if (hasFTarget && !(hasTTarget || hasFValue || hasFSet)) {
            step.type = MapStep::Type::RenameField;
        }
    } else if (hasFSet) {
        if (hasFValue && !(hasTTarget || hasFTarget)) {
            step.type = MapStep::Type::SetField;
        }
    }

    return step;
}

void parseMap (xml_node<> *node, vector<SourceMap> &maps) {
    EXPECT_NAME(node, "bcf:map");

    vector<u16string> forEntries {};
    unordered_set<u16string> notForEntries {};

    vector<MapStep> steps {};

    // TODO: Handle attributes

    for (auto *map = node->first_node(); map; map = map->next_sibling()) {
        const char *name = map->name();

        if (IS("per_datasource")) {
            // TODO: Handle different rules for different datasource. Ignore this attribute for now.
        } else if (IS("per_type")) {
            forEntries.emplace_back(utf.utf8to16(map->value()));
        } else if (IS("per_nottype")) {
            notForEntries.insert(utf.utf8to16(map->value()));
        } else if (IS("map_step")) {
            MapStep step = parseMapStep(map);
            if (step.type == MapStep::Type::Unhandled) {
                return;
            }
            steps.emplace_back(step);
        }
    }

    if (steps.empty()) {
        return;
    }

    SourceMap &sourcemap = maps.emplace_back(SourceMap { forEntries, notForEntries });

    u16string entryName {};
    bool entrySpecificMaps { false };

    auto itr = steps.begin();
    auto end = steps.end();

    for (; itr != end; itr++) {
        auto &step = *itr;
        switch (step.type) {
            case MapStep::Type::RenameEntry:
                sourcemap.entryAliases.emplace_back(std::pair { step.typeSource, step.typeTarget });
                if (step.final) {
                    entryName = step.typeSource;
                    entrySpecificMaps = true;
                }
                break;
            case MapStep::Type::RenameField:
                sourcemap.fieldAliases.emplace_back(std::pair { step.fieldSource, step.fieldTarget });
                break;
            case MapStep::Type::SetField:
                sourcemap.setFields.emplace_back(step.fieldSet); // ignore value; assume proper
            default: {}
        }

        if (entrySpecificMaps) break;
    }

    if (!entrySpecificMaps) return;

    itr++;
    for (; itr != end; itr++) {
        auto &step = *itr;
        if (step.final) return; // give up on nested final constraints...
        switch (step.type) {
            case MapStep::Type::RenameField:
                sourcemap.entryFieldAliases[entryName].emplace_back(std::pair { step.fieldSource, step.fieldTarget });
                break;
            case MapStep::Type::SetField:
                sourcemap.setEntryFields[entryName].emplace_back(step.fieldSet);
            default: {}
        }
    }
}

void Style::parseMaps (xml_node<> *def, vector<SourceMap> &maps) {
    EXPECT_NAME(def, "bcf:maps");

    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        parseMap(node, maps);
    }
}

void Style::parseDatamodel (xml_node<> *datamodel, vector<TempConstraintData> &tempConstraints, vector<u16string> &universalFields, unordered_map<u16string, vector<u16string>> &entryFields, unordered_map<u16string, Field> &fields) {
    EXPECT_NAME(datamodel, "bcf:datamodel");

    for (xml_node<> *node = datamodel->first_node(); node; node = node->next_sibling()) {
        const char *name = node->name();

        if (IS("constants")) {
            parseConstants(node);
        } else if (IS("entrytypes")) {
            parseEntryTypes(node);
        } else if (IS("fields")) {
            parseFields(node, fields);
        } else if (IS("entryfields")) {
            parseEntryFields(node, universalFields, entryFields);
        } else if (IS("constraints")) {
            parseConstraints(node, tempConstraints);
        } else {
            std::cerr << "Unhandled datamodel entry " << name << "\n";
        }
    }
}

void Style::parseConstants (xml_node<> *def) {
    EXPECT_NAME(def, "bcf:constants");

    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        EXPECT_NAME(node, "bcf:constant");

        Constant constant {};
        constant.value = utf.utf8to16(node->value());

        u16string name;

        for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            string attrValue = attr->value();

            if (attrName == "type") {
                if (attrValue == "list") constant.type = Constant::Type::List;
                // it is `string` by default
            } else {
                EXPECT_NAME(attr, "name");
                name = utf.utf8to16(attrValue);
            }
        }

        constants.insert({ name, constant });
    }
}

void Style::parseEntryTypes (xml_node<> *def) {
    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        EXPECT_NAME(node, "bcf:entrytype");

        u16string name = utf.utf8to16(node->value());
        Entry entry { name };

        auto *attr = node->first_attribute();
        if (attr) {
            // the schema only allows the attribute to be "skip_output"="true" if present
            EXPECT_NAME(attr, "skip_output");
            EXPECT_VAL(attr, "true");
            entry.skipOut = true;
        }

        assert(entries.count(name) == 0);
        entries.insert({ name, entry });
    }
}

void Style::parseFields (xml_node<> *def, unordered_map<u16string, Field> &fields) {
    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        EXPECT_NAME(node, "bcf:field");

        u16string name = utf.utf8to16(node->value());
        Field field { name };

        for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            string attrVal = attr->value();

            if (attrName == "fieldtype") {
                if (attrVal == "field") {
                    field.type = Field::Type::Field;
                } else {
                    assert(attrVal == "list");
                    field.type = Field::Type::List;
                }
            } else if (attrName == "datatype") {
                if (attrVal == "literal") {
                    field.dataType = Field::Data::Literal;
                } else if (attrVal == "name") {
                    field.dataType = Field::Data::Name;
                } else if (attrVal == "key") {
                    field.dataType = Field::Data::Key;
                } else if (attrVal == "entrykey") {
                    field.dataType = Field::Data::EntryKey;
                } else if (attrVal == "date") {
                    field.dataType = Field::Data::Date;
                } else if (attrVal == "verbatim") {
                    field.dataType = Field::Data::Verbatim;
                } else if (attrVal == "integer") {
                    field.dataType = Field::Data::Integer;
                } else if (attrVal == "range") {
                    field.dataType = Field::Data::Range;
                } else if (attrVal == "code") {
                    field.dataType = Field::Data::Code;
                } else if (attrVal == "uri") {
                    field.dataType = Field::Data::URI;
                } else if (attrVal == "datepart") {
                    field.dataType = Field::Data::Datepart;
                } else if (attrVal == "keyword") {
                    field.dataType = Field::Data::Keyword;
                } else if (attrVal == "option") {
                    field.dataType = Field::Data::Option;
                } else {
                    std::cerr << "Unhandled datatype " << attrVal << "\n";
                }
            } else if (attrName == "format") {
                if (attrVal == "xsv") {
                    field.format = Field::Format::XSV;
                } else {
                    std::cerr << "Unhandled format " << attrVal << "\n";
                    field.format = Field::Format::Other;
                }
            } else if (attrName == "nullok") {
                // NOTE: Technically the schema does not allow any other value besides "true",
                //  so the check is useless
                if (attrVal == "true") field.nullok = true;
            } else if (attrName == "skip_out") {
                if (attrVal == "true") field.skipout = true;
            } else if (attrName == "label") {
                if (attrVal == "true") field.label = true;
            }
        }

        fields.insert({ name, field });
    }
}

void Style::parseEntryFields (xml_node<> *def, vector<u16string> &universalFields, unordered_map<u16string, vector<u16string>> &entryFields) {
    vector<u16string> entryNames {};
    vector<u16string> fields {};

    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        const char *name = node->name();
        u16string value = utf.utf8to16(node->value());
        if (IS("field")) {
            fields.emplace_back(value);
        } else {
            EXPECT_NAME(node, "bcf:entrytype");
            entryNames.emplace_back(value);
        }
    }

    if (entryNames.empty()) {
        universalFields.insert(universalFields.end(), fields.begin(), fields.end());
    } else {
        for (u16string &entryName : entryNames) {
            entryFields[entryName] = fields;
        }
    }
}

void extractMandatoryConstraint (TempConstraintData &tempConstraint, xml_node<> *node) {
    EXPECT_NAME(node, "bcf:constraint");

    MandatoryConstraint &mandatory = tempConstraint.mandatory;

    for (xml_node<> *child = node->first_node(); child; child = child->next_sibling()) {
        const char *name = child->name();

        if (IS("field")) {
            mandatory.all.insert(utf.utf8to16(child->value()));
        } else if (IS("fieldor")) {
            vector<u16string> &fields = mandatory.some.emplace_back(vector<u16string> {});
            for (xml_node<> *field = child->first_node(); field; field = field->next_sibling()) {
                fields.emplace_back(utf.utf8to16(field->value()));
            }
        } else {
            assert(IS("fieldxor"));
            vector<u16string> &fields = mandatory.one.emplace_back(vector<u16string> {});
            for (xml_node<> *field = child->first_node(); field; field = field->next_sibling()) {
                fields.emplace_back(utf.utf8to16(field->value()));
            }
        }
    }
}

void extractConditionalConstraint (TempConstraintData &tempConstraint, xml_node<> *node) {
    EXPECT_NAME(node, "bcf:constraint");
    ConditionalConstraint &conditional = tempConstraint.conditional.emplace_back(ConditionalConstraint {});

    for (xml_node<> *child = node->first_node(); child; child = child->next_sibling()) {
        string attrVal = child->first_attribute()->value(); // TODO: Check and don't segfault on this

        ConditionalConstraint::Quant quant;

        if (attrVal == "all") {
            quant = ConditionalConstraint::Quant::All;
        } else if (attrVal == "one") {
            quant = ConditionalConstraint::Quant::One;
        } else {
            assert(attrVal == "none");
            quant = ConditionalConstraint::Quant::None;
        }

        vector<u16string> fields {};

        for (xml_node<> *field = child->first_node(); field; field = field->next_sibling()) {
            fields.emplace_back(utf.utf8to16(field->value()));
        }

        const char *name = child->name();
        if (IS("antecedent")) {
            conditional.pre.condition = quant;
            conditional.pre.fields = fields;
        } else {
            conditional.post.condition = quant;
            conditional.post.fields = fields;
        }
    }
}

void extractDataConstraint (TempConstraintData &tempConstraint, xml_node<> *node) {
    DataConstraint &data = tempConstraint.data.emplace_back(DataConstraint {});

    for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        string name = attr->name();
        string val = attr->value();
        if (name == "datatype") {
            if (val == "integer") {
                data.type = Field::ConstraintType::Integer;
            } else if (val == "isbn") {
                data.type = Field::ConstraintType::Isbn;
            } else if (val == "issn") {
                data.type = Field::ConstraintType::Issn;
            } else if (val == "ismn") {
                data.type = Field::ConstraintType::Ismn;
            } else if (val == "datepart") { // "date" is not actually a valid constraint (fixed in dev)
                data.type = Field::ConstraintType::Datepart;
            } else if (val == "pattern") {
                data.type = Field::ConstraintType::Pattern;
            }
        } else if (name == "rangemax") {
            data.rangeMax = std::stoi(val);
        } else if (name == "rangemin") {
            data.rangeMin = std::stoi(val);
        } else if (name == "pattern") {
            data.pattern = val;
        } else {
            assert(name == "type");
        }
    }

    for (xml_node<> *child = node->first_node(); child; child = child->next_sibling()) {
        data.fields.insert(utf.utf8to16(child->value()));
    }
}

void Style::parseConstraints (xml_node<> *def, vector<TempConstraintData> &constraints) {
    EXPECT_NAME(def, "bcf:constraints");

    TempConstraintData &tempConstraint = constraints.emplace_back(TempConstraintData {});

    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        const char *name = node->name();
        if (IS("entrytype")) {
            tempConstraint.entries.insert(utf.utf8to16(node->value()));
        } else {
            EXPECT_NAME(node, "bcf:constraint");
            auto *attr = node->first_attribute();
            assert(attr != nullptr);

            string attrVal = attr->value();

            // Note: These values can only appear in the separate types, so
            //  we don't need to check the attribute name (data is the fallback)
            if (attrVal == "mandatory") {
                extractMandatoryConstraint(tempConstraint, node);
            } else if (attrVal == "conditional") {
                extractConditionalConstraint(tempConstraint, node);
            } else {
                assert(attrVal == "data");
                extractDataConstraint(tempConstraint, node);
            }
        }
    }

}

void Style::buildEntryFields (vector<u16string> &universalFields, unordered_map<u16string, vector<u16string>> &entryFields, unordered_map<u16string, Field> &fields) {
    vector<Field> fieldsVec;
    fieldsVec.reserve(universalFields.size());
    for (u16string &fieldName : universalFields) {
        auto itr = fields.find(fieldName);
        if (itr == fields.end()) {
            std::cerr << "Missing field definition!!!\n";
            // TODO: Handle this
        }
        fieldsVec.emplace_back(itr->second);
    }

    for (auto &pair : entries) {
        Entry &entry = pair.second;
        for (size_t i = 0, length = fieldsVec.size(); i < length; i++) {
            entry.fields.insert({ universalFields[i], fieldsVec[i] });
        }
    }

    for (auto &pair : entryFields) {
        const u16string &entryName = pair.first;
        const vector<u16string> &fieldNames = pair.second;

        auto itr = entries.find(entryName);
        if (itr == entries.end()) continue;

        Entry &entry = itr->second;

        for (const u16string &fieldName : fieldNames) {
            auto itr2 = fields.find(fieldName);
            if (itr2 == fields.end()) continue;
            entry.fields.insert({ fieldName, itr2->second });
        }
    }
}

void addMandatoryConstraints (Entry &entry, MandatoryConstraint &mandatory) {
    Entry::Constraints &constraints = entry.constraints;
    constraints.all.insert(constraints.all.begin(), mandatory.all.begin(), mandatory.all.end());
    constraints.some.insert(constraints.some.begin(), mandatory.some.begin(), mandatory.some.end());
    constraints.one.insert(constraints.one.begin(), mandatory.one.begin(), mandatory.one.end());
}

void addConditionalConstraints (Entry &entry, vector<ConditionalConstraint> &conditional) {
    entry.constraints.conditional.insert(entry.constraints.conditional.end(), conditional.begin(), conditional.end());
}

void addDataConstraints (Entry &entry, vector<DataConstraint> &dataConstraints) {
    for (const DataConstraint &constraint : dataConstraints) {
        for (const u16string &fieldName : constraint.fields) {
            auto itr = entry.fields.find(fieldName);
            if (itr == entry.fields.end()) continue;
            Field &field = itr->second;

            field.constraintType = constraint.type;
            field.rangeMax = constraint.rangeMax;
            field.rangeMin = constraint.rangeMin;
            field.pattern = constraint.pattern;
        }
    }
}

void Style::addEntryConstraints (vector<TempConstraintData> &tempConstraints) {
    for (TempConstraintData &tempConstraint : tempConstraints) {
        unordered_set<u16string> &entryNames = tempConstraint.entries;

        if (!entryNames.empty()) {
            for (const u16string &entryName : entryNames) {
                auto itr = entries.find(entryName);
                if (itr == entries.end()) continue;

                Entry &entry = itr->second;

                addMandatoryConstraints(entry, tempConstraint.mandatory);
                addConditionalConstraints(entry, tempConstraint.conditional);
                addDataConstraints(entry, tempConstraint.data);
            }
        } else {
            // TODO: Handle global constraint
        }
    }
}

void consolidateConstraints (Entry &entry) {

}

void Style::consolidateEntryConstraints () {
    for (auto &pair : entries) {
        consolidateConstraints(pair.second);
    }
}

template<typename T>
bool contains (vector<T> &input, T element) {
    return std::find(input.begin(), input.end(), element) != input.end();
}

template<typename T>
vector<T> removeOccurrences (vector<T> &input, vector<T> toRemove) {
    vector<T> newList {};
    for (auto &elem : input) {
        std::cerr << "Testing " << elem << "\n";
        if (!contains(toRemove, elem)) {
            newList.push_back(elem);
        } else {
            std::cerr << "Removing " << elem << "\n";
        }
    }

    return newList;
}

std::optional<Entry *> Style::getEntry (const u16string &name) {
    auto itr = entries.find(name);
    if (itr == entries.end()) return {};
    return &itr->second;
}

std::ostream &operator << (std::ostream &out, const SourceMap &map) {
    out << "SourceMap: Set Entry Fields: " << map.setEntryFields.size();

    return out;
}

void addFieldAlias (Entry &entry, const u16string &alias, const u16string &actual) {
    auto itr = entry.fields.find(actual);
    if (itr == entry.fields.end()) return;

    Field actualFieldCopy = itr->second;
    actualFieldCopy.name = alias;

    entry.fields.insert({ alias, actualFieldCopy });


    // TODO: Well, you get the idea
    auto &constraints = entry.constraints;
    for (auto itr2 = constraints.all.begin(), end = constraints.all.end(); itr2 != end; itr2++) {
        if (*itr2 == actual) {
            constraints.all.erase(itr2);
            constraints.one.emplace_back(vector { actual, alias });
            return;
        }
    }

    for (auto &fields : constraints.some) {
        if (std::find(fields.begin(), fields.end(), actual) != fields.end()) {
            fields.emplace_back(alias);
            return;
        }
    }

    for (auto &fields : constraints.one) {
        if (std::find(fields.begin(), fields.end(), actual) != fields.end()) {
            fields.emplace_back(alias);
            return;
        }
    }
}

void Style::applySourcemaps (vector<SourceMap> sourcemaps) {
    for (SourceMap &sourcemap : sourcemaps) {
        std::cerr << sourcemap << "\n";
        for (const auto &pair : sourcemap.entryAliases) {
            const u16string &alias = pair.first;
            const u16string &actual = pair.second;

            auto itr = entries.find(actual);
            if (itr == entries.end()) continue;

            Entry entryCopy = itr->second.copy();
            entries.insert({ alias, entryCopy });
        }

        for (const auto &pair : sourcemap.setEntryFields) {
            const auto &entryName = pair.first;
            const auto &setFields = pair.second;

            auto itr = entries.find(entryName);
            if (itr == entries.end()) continue;
            Entry &entry = itr->second;

            auto &constraints = entry.constraints;

            constraints.all = removeOccurrences(constraints.all, setFields);

        }

        for (const auto &pair : sourcemap.fieldAliases) {
            const u16string &alias = pair.first;
            const u16string &actual = pair.second;

            for (auto &entryPair : entries) {
                Entry &entry = entryPair.second;
                addFieldAlias(entry, alias, actual);
            };
        }
    }
}

#undef EXPECT_VAL
#undef EXPECT_NAME
#undef IS

}

