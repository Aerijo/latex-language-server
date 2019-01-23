#include <iostream>
#include <filesystem/File.h>
#include <filesystem/FileManager.h>
#include <biber/BibIndexer.h>
#include "FileOpenHandler.h"

void FileOpenHandler::run (optional<Value> &params) {
    if (!params) { handleMissingFileOpenParams(); return; }

    Value &value = *params;

    Value &textDocument = value["textDocument"];

    string uri = textDocument["uri"].GetString();

    std::cerr << "handling opening file " << uri << "\n";

    string languageId = textDocument["languageId"].GetString();

    versionNum version = textDocument["version"].GetUint64();

    std::cerr << "Opened " << uri << " with ID:" << languageId << ", v" << version << "\n";

    string text = textDocument["text"].GetString();

    File *file = new File { uri, languageId, version, text };

    file->setupParser();

    /*
     * Steps to parse:
     * 1. Create a TSInput
     *  a. Make a TextBufferInput from snapshot slices (primitive_chunks should work)
     *  b. Get it from ->input()
     * 2. With this, the parser, and the old tree, call ts_parser_parse
     */

    FileManager::add(uri, file);

    if (file->hasParser) {
        BibIndexer indexer { file };

        indexer.completeIndex();
    }

}

void FileOpenHandler::handleMissingFileOpenParams () {
    std::cerr << "Missing file open params!! Possibly out of sync!!\n";

    // TODO: Request resync when this happens
}
