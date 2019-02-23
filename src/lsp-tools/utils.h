#ifndef LATEX_LANGUAGE_SERVER_UTILS_H
#define LATEX_LANGUAGE_SERVER_UTILS_H

#include <rapidjson/document.h>
#include <range.h>
#include <tree_sitter/runtime.h>

TSPoint getTSPointFromJSON (rapidjson::Value &point);

Point getPointFromJSON (rapidjson::Value &point);

Range getRangeFromJSON (rapidjson::Value &range);

#endif //LATEX_LANGUAGE_SERVER_UTILS_H
