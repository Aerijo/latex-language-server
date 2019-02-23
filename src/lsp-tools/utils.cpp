#include "./utils.h"
#include "utils.h"

TSPoint getTSPointFromJSON (rapidjson::Value &point) {
    unsigned line = point["line"].GetUint();
    unsigned character = point["character"].GetUint() << 1;

    return TSPoint {line, character};
}

Point getPointFromJSON (rapidjson::Value &point) {
    unsigned line = point["line"].GetUint();
    unsigned character = point["character"].GetUint();

    return Point { line, character };
}

Range getRangeFromJSON (rapidjson::Value &range) {
    rapidjson::Value &start = range["start"];
    rapidjson::Value &end = range["end"];

    unsigned startLine = start["line"].GetUint();
    unsigned startChar = start["character"].GetUint();
    unsigned endLine = end["line"].GetUint();
    unsigned endChar = end["character"].GetUint();

    return Range { { startLine, startChar }, { endLine, endChar } };
}

