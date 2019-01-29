#include <iostream>
#include "Style.h"

#include <cstring>

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

template <typename T>
std::ostream &operator << (std::ostream &out, const vector<T> &input) {
    out << "[";
    for (const auto &entry : input) {
        out << entry << ", ";
    }
    if (!input.empty()) out << "\b\b";
    out << "]";
    return out;
}

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

std::ostream &operator << (std::ostream &out, const Entry &input) {
    out << ":" << input.name << " F=" << input.fields.size() << " C=" << input.constraints.all.size() << "-" << input.constraints.some.size() << "-" << input.constraints.one.size() <<  ":";
    return out;
}

void applyGlobalDataConstraints (unordered_map<u16string, Field> &fields, vector<TempConstraintData> &tempConstraints) {
//    for (const TempConstraintData &tempConstraint : tempConstraints) {
//        if (tempConstraint.data.empty() || !tempConstraint.entries.empty()) continue;
//
//
//
//        for (const u16string &fieldName : tempConstraint.fields) {
//            auto itr = fields.find(fieldName);
//            if (itr == fields.end()) continue; // TODO: Handle missing field
//            Field &field = itr->second;
//
//
//        }
//    }
}

Style::Style (xml_document<> &doc) {
    std::cerr << "constructing Style from xml...\n";
    EXPECT_NAME(doc.first_node(), "bcf:controlfile");
    // NOTE: bcf:options component="biber" contains encoding information.
    //  We will assume it's always UTF-8 for now.

    vector<TempConstraintData> tempConstraints {};
    vector<u16string> universalFields {};
    unordered_map<u16string, vector<u16string>> entryFields {};
    unordered_map<u16string, Field> fields {};

    for (xml_node<> *node = doc.first_node()->first_node(); node; node = node->next_sibling()) {
        const char *name = node->name();
         if (IS("datamodel")) {
            parseDatamodel(node, tempConstraints, universalFields, entryFields, fields);
        }
    }

    applyGlobalDataConstraints(fields, tempConstraints);
}

void Style::parseDatamodel (xml_node<> *datamodel, vector<TempConstraintData> &tempConstraints, vector<u16string> &universalFields, unordered_map<u16string, vector<u16string>> &entryFields, unordered_map<u16string, Field> &fields) {
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
    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        EXPECT_NAME(node, "bcf:constant");

        Constant constant {};
        constant.value = utf.utf8to16(node->value());

        u16string name;

        for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            u16string attrValue = utf.utf8to16(attr->value());

            if (attrName == "type") {
                if (attrValue == u"list") constant.type = Constant::Type::List;
                // it is `string` by default
            } else {
                EXPECT_NAME(attr, "name");
                name = attrValue;
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

void addMandatoryConstraint (TempConstraintData &data, xml_node<> *node) {
    EXPECT_NAME(node, "bcf:constraint");
    UtfHandler utf {};

    MandatoryConstraint &mandatory = data.mandatory.emplace_back(MandatoryConstraint {});

    for (xml_node<> *child = node->first_node(); child; child = child->next_sibling()) {
        const char *name = child->name();

        if (IS("field")) {
            mandatory.all.emplace_back(UtfHandler().utf8to16(child->value()));
        } else if (IS("fieldor")) {
            vector<u16string> &fields = mandatory.some.emplace_back(vector<u16string> {});
            for (xml_node<> *field = child->first_node(); field; field = field->next_sibling()) {
                fields.emplace_back(utf.utf8to16(child->value()));
            }

        } else {
            assert(IS("fieldxor"));
            vector<u16string> &fields = mandatory.one.emplace_back(vector<u16string> {});
            for (xml_node<> *field = child->first_node(); field; field = field->next_sibling()) {
                fields.emplace_back(utf.utf8to16(child->value()));
            }
        }
    }
}

void addConditionalConstraint (TempConstraintData &data, xml_node<> *node) {
    UtfHandler utf {};
    ConditionalConstraint &conditional = data.conditional.emplace_back(ConditionalConstraint {});

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

    data.conditional.emplace_back(conditional);
}

void addDataConstraint (TempConstraintData &tempConstraint, xml_node<> *node) {
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
        data.fields.emplace_back(utf.utf8to16(child->value()));
    }
}

void Style::parseConstraints (xml_node<> *def, vector<TempConstraintData> &constraints) {
    EXPECT_NAME(def, "bcf:constraints");

    TempConstraintData data {};

    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        const char *name = node->name();
        if (IS("entrytype")) {
            data.entries.emplace_back(utf.utf8to16(node->value()));
        } else {
            EXPECT_NAME(node, "bcf:constraint");
            auto *attr = node->first_attribute();
            assert(attr != nullptr);

            string attrVal = attr->value();

            // Note: These values can only appear in the separate types, so
            //  we don't need to check the attribute name (data is the fallback)
            if (attrVal == "mandatory") {
                addMandatoryConstraint(data, node);
            } else if (attrVal == "conditional") {
                addConditionalConstraint(data, node);
            } else {
                assert(attrVal == "data");
                addDataConstraint(data, node);
            }
        }
    }

    constraints.emplace_back(data);
}

#undef EXPECT_VAL
#undef EXPECT_NAME
#undef IS

}

