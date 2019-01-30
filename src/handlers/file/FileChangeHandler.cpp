#include <filesystem/File.h>
#include <filesystem/FileManager.h>
#include <biber/BibIndexer.h>
#include <fstream>
#include <lconfig.h>
#include "FileChangeHandler.h"

void FileChangeHandler::registerCapabilities (Init::ServerCapabilities &capabilities) {
    if (!capabilities.textDocumentSync) capabilities.textDocumentSync = Init::TextDocumentSyncOptions {};

    capabilities.textDocumentSync->openClose = true;

    capabilities.textDocumentSync->change = TextDocumentSyncKind::Incremental;

    capabilities.textDocumentSync->willSave = true;

    capabilities.textDocumentSync->willSaveWaitUntil = false;
}

optional<string> findBiberDataModel () {
    return optional { std::string { "../test/resources/project1/main.bcf" } };
}

Range getRangeFromJSON (Value &range) {
    Value &start = range["start"];
    Value &end = range["end"];

    unsigned startLine = start["line"].GetUint();
    unsigned startChar = start["character"].GetUint();
    unsigned endLine = end["line"].GetUint();
    unsigned endChar = end["character"].GetUint();

    return Range { { startLine, startChar }, { endLine, endChar } };
}

void FileChangeHandler::run (optional<Value> &params) {
    if (!params) { handleMissingFileChangeParams(); return; }

    bool hasVersion { false };
    versionNum version { 0 };

    Value &value = *params;

    Value &textDocument = value["textDocument"];

    if (textDocument["version"].IsNumber()) {
        version = textDocument["version"].GetInt64();
        hasVersion = true;
    } else {
        assert(textDocument["version"].IsNull());
    }

    string uri = textDocument["uri"].GetString();

    File *file = FileManager::get(uri);
    assert(file != nullptr);

    if (hasVersion) {
        if (!file->validNextVersion(version)) {
            std::cerr << "!!! Out of sync file versioning !!!\n";
        }
        file->setVersion(version);
    } else {
        std::cerr << "No version num for change\n"; // NOTE: Not necessarily an error
    }

    Value &contentChanges = value["contentChanges"];

    for (auto &change : contentChanges.GetArray()) {
        string text = change["text"].GetString();

        if (change.HasMember("range")) {
            Value &range = change["range"];
            file->setTextInRange(getRangeFromJSON(range), std::move(text));
        } else {
            text = change["text"].GetString();
            file->setText(std::move(text));
        }
    }

    if (file->hasParser) {
        auto datamodel = findBiberDataModel();

        if (datamodel) {
            BibIndexer indexer { file, g_config->bibtex.style };

            indexer.completeIndex();
        }
    }
}

void FileChangeHandler::handleMissingFileChangeParams () {
    // TODO
}
