#include <iostream>
#include "Style.h"

#include <cstring>

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
 *
 * (Handled) Types of Map:
 * - map_type_source -> map_type_target (entry rename; create pseudo entry in style)
 * - when the above matches, map_final="1", and map_field_set -> map_field_value (account for this by removing from constraints)
 * - map_field_source -> map_field_target
 */

#define IS(input) strcmp(name + 4, input) == 0 // all names start with `bcf:`
#define EXPECT_NAME(node, input) assert(strcmp(node->name(), input) == 0)
#define EXPECT_VAL(node, input) assert(strcmp(node->value(), input) == 0)

std::ostream & operator << (std::ostream &out, const Field &field) {
    UtfHandler utf {};

    out
    << utf.utf16to8(field.name) << ": "
    << (int) field.type << ", "
    << (int) field.format << ", "
    << (int) field.dataType << ", "
    << field.nullok << ", "
    << field.skipout << ", "
    << field.label;

    return out;
}

std::ostream & operator << (std::ostream &out, const u16string &input) {
    UtfHandler utf{};
    out << utf.utf16to8(input);
    return out;
}

Style::Style (xml_document<> &doc) {
    std::cerr << "constructing Style from xml...\n";

    EXPECT_NAME(doc.first_node(), "bcf:controlfile");

    // NOTE: bcf:options component="biber" contains encoding information.
    //  We will assume it's always UTF-8 for now.

    vector<xml_node<> *> options;
    vector<xml_node<> *> optionScope;
    vector<xml_node<> *> dataFieldSet;
    vector<xml_node<> *> sourceMap;
    vector<xml_node<> *> inheritance;
    vector<xml_node<> *> datamodel;

    // from datamodel
    vector<xml_node<> *> constraints;

    // processed
    vector<u16string> universalFields;
    map<u16string, vector<u16string>> entryFields;

    for (xml_node<> *node = doc.first_node()->first_node(); node; node = node->next_sibling()) {
        const char *name = node->name();

        if (IS("options")) {
            options.push_back(node);
        } else if (IS("optionscope")) {
            optionScope.push_back(node);
        } else if (IS("datafieldset")) {
            dataFieldSet.push_back(node);
        } else if (IS("sourcemap")) {
            sourceMap.push_back(node);
        } else if (IS("inheritance")) {
            inheritance.push_back(node);
        } else if (IS("datamodel")) {
            parseDatamodel(node, constraints, universalFields, entryFields);
        }

    }


    // NOTE: Printing out all the gathered data:
    std::cerr << "Fields: ";
    for (const auto &field : universalFields) {
        std::cerr << field << ", ";
    }
    std::cerr << "\b\b\n\n";

    std::cerr << "Entry fields:\n";
    for (const auto &pair : entryFields) {
        std::cerr << "- " << pair.first << ": ";
        for (auto &f : pair.second) {
            std::cerr << f << ", ";
        }
        std::cerr << "\b\b\n";
    }

    std::cerr << "Fields:\n";
    for (const auto &pair : fields) {
        std::cerr << pair.second << "\n";
    }
    std::cerr << "\n";

    std::cerr << "Constants:\n";
    for (const auto &pair : constants) {
        std::cerr << pair.first << ": " << (int) pair.second.type << ", " << pair.second.value << "\n";
    }
}

void Style::parseDatamodel (xml_node<> *datamodel, vector<xml_node<> *> &constraints, vector<u16string> &universalFields, map<u16string, vector<u16string>> &entryFields) {
    for (xml_node<> *node = datamodel->first_node(); node; node = node->next_sibling()) {
        const char *name = node->name();

        if (IS("constants")) {
            parseConstants(node);
        } else if (IS("entrytypes")) {
            parseEntryTypes(node);
        } else if (IS("fields")) {
            parseFields(node);
        } else if (IS("entryfields")) {
            parseEntryFields(node, universalFields, entryFields);
        } else if (IS("constraints")) {
            parseConstraints(node, constraints);
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
                if (attrValue == u"list") constant.type = ConstantType::List;
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

void Style::parseFields (xml_node<> *def) {
    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        EXPECT_NAME(node, "bcf:field");

        u16string name = utf.utf8to16(node->value());
        Field field { name };

        for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            string attrVal = attr->value();

            if (attrName == "fieldtype") {
                if (attrVal == "field") {
                    field.type = FieldType::Field;
                } else {
                    assert(attrVal == "list");
                    field.type = FieldType::List;
                }
            } else if (attrName == "datatype") {
                if (attrVal == "literal") {
                    field.dataType = DataType::Literal;
                } else if (attrVal == "name") {
                    field.dataType = DataType::Name;
                } else if (attrVal == "range") {
                    field.dataType = DataType::Range;
                } else if (attrVal == "integer") {
                    field.dataType = DataType::Integer;
                } else if (attrVal == "datepart") {
                    field.dataType = DataType::Datepart;
                } else if (attrVal == "date") {
                    field.dataType = DataType::Date;
                } else if (attrVal == "verbatim") {
                    field.dataType = DataType::Verbatim;
                } else if (attrVal == "uri") {
                    field.dataType = DataType::URI;
                } else if (attrVal == "separatedvalue") { // TODO: Find any usage of this...
                    field.dataType = DataType::SeparatedValue;
                } else if (attrVal == "key") {
                    field.dataType = DataType::Key;
                } else if (attrVal == "entrykey") {
                    field.dataType = DataType::EntryKey;
                } else if (attrVal == "keyword") {
                    field.dataType = DataType::Keyword;
                } else if (attrVal == "option") {
                    field.dataType = DataType::Option;
                } else if (attrVal == "code") {
                    field.dataType = DataType::Code;
                } else if (attrVal == "pattern") {
                    field.dataType = DataType::Pattern;
                } else {
                    std::cerr << "Unhandled datatype " << attrVal << "\n";
                }
            } else if (attrName == "format") {
                if (attrVal == "xsv") {
                    field.format = Format::XSV;
                } else {
                    std::cerr << "Unhandled format " << attrVal << "\n";
                    field.format = Format::Other;
                }
            } else if (attrName == "nullok") {
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

void Style::parseEntryFields (xml_node<> *def, vector<u16string> &universalFields, map<u16string, vector<u16string>> &entryFields) {
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

void Style::parseConstraints (xml_node<> *def, vector<xml_node<> *> &constraints) {
    for (xml_node<> *node = def->first_node(); node; node = node->next_sibling()) {
        constraints.push_back(node);
    }
}

#undef EXPECT_VAL
#undef EXPECT_NAME
#undef IS

}

