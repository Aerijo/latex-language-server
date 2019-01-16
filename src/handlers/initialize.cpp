#include <iostream>

#include "initialize.h"
#include "definitions.h"
#include "messaging.h"

namespace Init {
    void reflectBool (Writer<StringBuffer> &writer, const string &name, bool value, bool skipIfFalse) {
        if (skipIfFalse && !value) return;

        writer.Key(name);
        writer.Bool(value);
    }

    void reflectInt (Writer<StringBuffer> &writer, const string &name, int value, bool skipIfNegative) {
        if (skipIfNegative && value < 0) return;

        writer.Key(name);
        writer.Int(value);
    }

    void reflectObject (Writer<StringBuffer> &writer, const string &name, Reflectable &value) {
        writer.Key(name);
        writer.StartObject();
        value.reflect(writer);
        writer.EndObject();
    }

    void reflectString (Writer<StringBuffer> &writer, const string &name, string &value, bool skipIfEmpty) {
        if (skipIfEmpty && value.empty()) return;

        writer.Key(name);
        writer.String(value);
    }

    template<>
    void reflectVector<string> (StringWriter &writer, const string &name, vector<string> &list, bool skipIfEmpty) {
        if (skipIfEmpty && list.empty()) return;

        writer.Key(name);
        writer.StartArray();
        for (string &elem : list) {
            writer.String(elem);
        }
        writer.EndArray();
    }
}


void InitializeHandler::run (Id &id, optional<Value> &params) {
    std::cerr << "handling initialisation request...\n";

    INIT_WRITER

    writer.Key("id"); id.writeId(writer);

    writer.Key("result"); writer.StartObject();

    writer.Key("capabilities");

    Init::ServerCapabilities foo {};

    foo.reflect(writer);


    writer.EndObject();

    SEND_MESSAGE
}
