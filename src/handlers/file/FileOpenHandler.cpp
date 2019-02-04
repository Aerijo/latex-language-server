#include <iostream>
#include <filesystem/File.h>
#include <filesystem/FileManager.h>
#include <biber/BibIndexer.h>
#include <lconfig.h>
#include "FileOpenHandler.h"



void FileOpenHandler::run (optional<Value> &params) {
    if (!params) { handleMissingFileOpenParams(); return; }

    Value &value = *params;

    Value &textDocument = value["textDocument"];

    string uri = textDocument["uri"].GetString();

    string languageId = textDocument["languageId"].GetString();

    versionNum version = textDocument["version"].GetUint64();

    std::cerr << "Opened " << uri << " with ID:" << languageId << ", v" << version << "\n";

    string text = textDocument["text"].GetString();

    File *file = new File { uri, languageId, version, text };

    file->setupParser();

    FileManager::add(uri, file);

    if (file->hasParser) {
        BibIndexer indexer { file, g_config->bibtex.style };

        indexer.completeIndex();
    }

}

void FileOpenHandler::handleMissingFileOpenParams () {
    std::cerr << "Missing file open params!! Possibly out of sync!!\n";

    // TODO: Request resync when this happens
}
