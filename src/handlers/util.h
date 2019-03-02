#ifndef LATEX_LANGUAGE_SERVER_UTIL_H
#define LATEX_LANGUAGE_SERVER_UTIL_H

#include <point.h>
#include <range.h>
#include <tree_sitter/runtime.h>

Point fromTSPoint (TSPoint tsPoint);

TSPoint toTSPoint (Point point);

Range fromTSRange (TSRange tsRange);

TSRange toTSRange (Range range);

TSRange ts_node_range (TSNode node); // form emulates existing Tree-sitter functions

Range nodeRange (TSNode node);

#endif //LATEX_LANGUAGE_SERVER_UTIL_H
