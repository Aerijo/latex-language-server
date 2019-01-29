#include "UtfHandler.h"

#include <vector>

using std::vector;

u16string UtfHandler::utf8to16 (const string &input) {
    try {
        return converter.from_bytes(input);
    } catch (...) {
        return u"FAILED :("; // TODO: Add options to recover from this (e.g., resync entire file -- pass handling routine as lambda)
    }
}

string UtfHandler::utf16to8 (const u16string &input) {
    try {
        return converter.to_bytes(input);
    } catch (...) {
        return "FAILED :(";
    }
}
