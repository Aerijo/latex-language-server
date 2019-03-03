#include <filesystem/File.h>
#include <filesystem/FileManager.h>
#include <biber/BibIndexer.h>
#include <fstream>
#include <lconfig.h>
#include "FileChangeHandler.h"
#include "lsp-tools/utils.h"

void FileChangeHandler::registerCapabilities (Init::ServerCapabilities &capabilities) {
    if (!capabilities.textDocumentSync) capabilities.textDocumentSync = Init::TextDocumentSyncOptions {};
    auto &textSync = *capabilities.textDocumentSync;

    textSync.openClose = true;
    textSync.change = TextDocumentSyncKind::Incremental;
    textSync.willSave = true;
    textSync.willSaveWaitUntil = false;
}

optional<string> findBiberDataModel () {
    return optional { std::string { "../test/resources/project1/main.bcf" } };
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
            Value &rangeVal = change["range"];
            Range range = getRangeFromJSON(rangeVal);
            file->setTextInRange(range, std::move(text));
        } else {
            text = change["text"].GetString();
            file->setText(std::move(text));
        }
    }

    if (file->type == File::Type::Bib && file->hasParser) {
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
