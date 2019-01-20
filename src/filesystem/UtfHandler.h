#ifndef LATEX_LANGUAGE_SERVER_UTFHANDLER_H
#define LATEX_LANGUAGE_SERVER_UTFHANDLER_H

#include <string>
#include <encoding-conversion.h>

#include <codecvt>
#include <locale>

using std::string;
using std::u16string;

class UtfHandler {
private:
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter {};

public:
    UtfHandler () = default;

    // All I ever wanted... why is C++ so verbose ^^^ and inflexible :(

    u16string utf8to16 (string &input);

    string utf16to8 (u16string &input);
};

#endif //LATEX_LANGUAGE_SERVER_UTFHANDLER_H
