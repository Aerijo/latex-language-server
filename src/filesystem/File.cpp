#include "File.h"

using std::u16string;

bool File::operator == (File &that) {
    return uri == that.uri;
}

File::File (Uri &uri, string &languageId, string &text) : uri { uri }, languageId { languageId } {
    u16string converted = utf.utf8to16(text);
    buffer.set_text(move(converted));
}

File::File (string &uri, string &languageId, string &text) : uri { Uri::parse(uri) }, languageId { languageId } {
    u16string converted = utf.utf8to16(text);
    buffer.set_text(move(converted));
}

void File::print (std::ostream &stream) {
    u16string text = buffer.text();

    stream << utf.utf16to8(text);
}

void File::setTextInRange (Range oldRange, std::u16string &&text) {
    buffer.set_text_in_range(oldRange, move(text));
}


