#include "File.h"
#include "TextBufferInput.h"

extern "C" { TSLanguage *tree_sitter_biber(); }

using std::u16string;

bool File::operator == (File &that) {
    return uri == that.uri;
}

File::File (Uri &uri, string &languageId, string &text) : uri { uri }, languageId { languageId } {
    buffer.set_text(utf.utf8to16(text));
}

File::File (string &uri, string &languageId, versionNum version, string &text) : uri { Uri::parse(uri) }, languageId { languageId }, version { version } {
    buffer.set_text(utf.utf8to16(text));
}

void File::print (std::ostream &stream) {
    u16string text = buffer.text();
    stream << utf.utf16to8(text);
}

void File::setTextInRange (Range oldRange, std::string &&text) {
    setTextInRange(oldRange, utf.utf8to16(text));
}

TSPoint TSEndPointOfText (u16string &text) {
    uint32_t newlines = 0;
    auto length = static_cast<uint32_t>(text.length());

    uint32_t lastNewlineIndex = 0;

    for (uint32_t i = 0; i < length; i++) {
        auto c = text[i];

        switch (c) {
            case '\r':
                newlines++;
                lastNewlineIndex = i;
                if (i + 1 < length && text[i + 1] == '\n') { i++; lastNewlineIndex++; }
                break;
            case '\n':
                newlines++;
                lastNewlineIndex = i;
                break;
            default: {}
        }
    }

    return TSPoint { newlines, length - lastNewlineIndex - 1 };
}


void File::setTextInRange (Range oldRange, std::u16string &&text) {
    if (hasParser) {
        auto startIndex = buffer.clip_position(oldRange.start).offset;
        u16string oldText = buffer.text_in_range(oldRange);

        Point start = oldRange.start;
        Point end = oldRange.end;

        TSPoint newEndPoint = TSEndPointOfText(text);

        if (newEndPoint.row == 0) {
            newEndPoint.column += start.column;
        }

        newEndPoint.row += start.row;

        TSInputEdit edit = {
                startIndex, // start byte
                startIndex + static_cast<unsigned>(oldText.length()), // old end byte
                startIndex + static_cast<unsigned>(text.length()), // new end byte
                TSPoint { start.row, start.column }, // start point
                TSPoint { end.row, end.column }, // old end point
                newEndPoint // new end point
        };

        ts_tree_edit(tree, &edit);
    }

    buffer.set_text_in_range(oldRange, move(text));

    if (hasParser) {
        executeParse();

//        std::cerr << "Updated parse tree\n";
//        TSNode root_node = ts_tree_root_node(tree);
//
//        char *string = ts_node_string(root_node);
//
//        std::cerr << "Syntax tree:\n" << string << "\n";
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

    tree = ts_parser_parse(parser, nullptr, textBufferInput.input());

    delete snapshot;
}

void File::setupParser () {
    if (languageId != "biber" && languageId != "bibtex" && languageId != "tree biber") return;

    std::cerr << "setting up parser for "; uri.print(); std::cerr << "\n";

    hasParser = true;

    parser = ts_parser_new();

    ts_parser_set_language(parser, tree_sitter_biber());

    executeParse();

    TSNode root_node = ts_tree_root_node(tree);

    char *string = ts_node_string(root_node);

    std::cerr << "Syntax tree:\n" << string << "\n";
}

TSTree *File::getParseTree () {
    return ts_tree_copy(tree);
}

string File::getPath () {
    return uri.getPath();
}


