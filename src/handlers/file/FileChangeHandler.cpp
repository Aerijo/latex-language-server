#include <filesystem/File.h>
#include <filesystem/FileManager.h>
#include <biber/BibIndexer.h>
#include "FileChangeHandler.h"

void FileChangeHandler::registerCapabilities (Init::ServerCapabilities &capabilities) {
    if (!capabilities.textDocumentSync) capabilities.textDocumentSync = Init::TextDocumentSyncOptions {};

    capabilities.textDocumentSync->openClose = true;

    capabilities.textDocumentSync->change = TextDocumentSyncKind::Incremental;

    capabilities.textDocumentSync->willSave = true;

    capabilities.textDocumentSync->willSaveWaitUntil = false;
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
            Value &start = range["start"];
            Value &end = range["end"];

            unsigned startLine = start["line"].GetUint();
            unsigned startChar = start["character"].GetUint();
            unsigned endLine = end["line"].GetUint();
            unsigned endChar = end["character"].GetUint();

            file->setTextInRange({{ startLine, startChar }, { endLine, endChar }}, std::move(text));
        } else {
            text = change["text"].GetString();
            file->setText(std::move(text));
        }
    }

    if (file->hasParser) {
        BibIndexer indexer { file };

        indexer.completeIndex();
    }
}

void FileChangeHandler::handleMissingFileChangeParams () {
    // TODO
}
