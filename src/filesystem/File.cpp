#include <handlers/util.h>
#include "File.h"
#include "TextBufferInput.h"

extern "C" {
    TSLanguage *tree_sitter_biber();
    TSLanguage *tree_sitter_latex();
}

using std::u16string;

bool File::operator == (File &that) {
    return uri == that.uri;
}

File::Type getFileTypeFromLanguageId (string &languageId) {
    if (languageId == "bibtex" || languageId == "biber") {
        return File::Type::Bib;
    } else if (languageId == "latex" || languageId == "latex2e") {
        return File::Type::Tex;
    }

    return File::Type::Other;
}

File::File (Uri::Uri &uri, string &languageId, string &text) : uri { uri }, languageId { languageId }, type { getFileTypeFromLanguageId(languageId) } {
    buffer.set_text(utf.utf8to16(text));
}

File::File (string &uri, string &languageId, versionNum version, string &text) : uri { Uri::Uri::parse(uri) }, languageId { languageId }, version { version }, type { getFileTypeFromLanguageId(languageId) } {
    buffer.set_text(utf.utf8to16(text));
}

void File::print (std::ostream &stream) {
    u16string text = buffer.text();
    stream << utf.utf16to8(text);
}

void File::setTextInRange (Range &oldRange, std::string &&text) {
    setTextInRange(oldRange, utf.utf8to16(text));
}

/*
 *    0   1   2   3   4
 * 0:   a | b | c | \n
 *
 *    4   5   6   7
 * 1:   d | e | f
 *
 * When we are on char i [0:a, 1:b, ...], we want the newline position to be registered as the
 * index after (so 4 in this example, where i would be 3). This gives character == length - newline position,
 * even when there are no newlines in the text.
 */
TSPoint TSEndPointOfText (u16string &text) {
    if (text.empty()) return TSPoint { 0, 0 };

    uint32_t newlines = 0;
    auto length = static_cast<uint32_t>(text.length());

    uint32_t lastNewlineIndex = 0;

    for (uint32_t i = 0; i < length; i++) {
        auto c = text[i];

        switch (c) {
            case '\r':
                newlines++;
                lastNewlineIndex = i + 1;
                if (i + 1 < length && text[i + 1] == '\n') { i++; lastNewlineIndex++; }
                break;
            case '\n':
                newlines++;
                lastNewlineIndex = i + 1;
                break;
            default: {}
        }
    }

    return TSPoint { newlines, (length - lastNewlineIndex) << 1 };
}

void File::setTextInRange (Range &oldRange, std::u16string &&text) {
    if (hasParser) {
        auto startIndex = buffer.clip_position(oldRange.start).offset;
        u16string oldText = buffer.text_in_range(oldRange);

        TSPoint start = toTSPoint(oldRange.start);
        TSPoint end = toTSPoint(oldRange.end);

        TSPoint newEndPoint = TSEndPointOfText(text);

        if (newEndPoint.row == 0) {
            newEndPoint.column += start.column;
        }

        newEndPoint.row += start.row;

        TSInputEdit edit = {
                startIndex, // start byte
                startIndex + (static_cast<unsigned>(oldText.length()) << 1), // old end byte
                startIndex + (static_cast<unsigned>(text.length()) << 1), // new end byte
                start, // start point
                end, // old end point
                newEndPoint // new end point
        };

        ts_tree_edit(tree, &edit);
    }

    buffer.set_text_in_range(oldRange, move(text));

    if (hasParser) {
        executeParse();
    }
}

bool File::validNextVersion (versionNum nextVersion) {
    return nextVersion > version;
}

void File::setVersion (versionNum nextVersion) {
    version = nextVersion;
}

void File::setText (std::string &&text) {
    buffer.set_text(utf.utf8to16(text));

    if (hasParser) {
        ts_tree_delete(tree);
        tree = nullptr;
        executeParse();
    }
}

File::~File () {
    if (hasParser) {
        ts_tree_delete(tree);
        ts_parser_delete(parser);
    }
}

void File::executeParse () {
    auto snapshot = buffer.create_snapshot(); // NOTE: Looks like this can be done without creating a snapshot

    auto chunks = snapshot->primitive_chunks();

    TextBufferInput textBufferInput { &chunks };

    tree = ts_parser_parse(parser, tree, textBufferInput.input());

    delete snapshot;
}

void File::setupParser () {
    switch (type) {
        case File::Type::Bib:
            parser = ts_parser_new();
            ts_parser_set_language(parser, tree_sitter_biber());
            break;
        case File::Type::Tex:
            parser = ts_parser_new();
            ts_parser_set_language(parser, tree_sitter_latex());
            break;
        default: return;
    }

    hasParser = true;
    executeParse();
}

TSTree *File::getParseTree () {
    return ts_tree_copy(tree);
}

string File::getPath () {
    return uri.getPath();
}

Range rangeForNode (const TSNode &node) {
    TSPoint startPoint = ts_node_start_point(node);
    TSPoint endPoint = ts_node_end_point(node);

    return Range { { startPoint.row, startPoint.column >> 1 }, { endPoint.row, endPoint.column >> 1 } };
}

u16string File::textForNode (const TSNode &node) {
    Range range = rangeForNode(node);
    return textInRange(range);
}

string File::utf8TextForNode (const TSNode &node) {
    Range range = rangeForNode(node);
    return utf8TextInRange(range);
}

u16string File::textInRange (const Range &range) {
    return buffer.text_in_range(range);
}

string File::utf8TextInRange (const Range &range) {
    return utf.utf16to8(buffer.text_in_range(range));
}

TSNode File::getRootNode () {
    return ts_tree_root_node(tree);
}

Point File::getEndPoint () {
    return buffer.extent();
}


