#include "FileChangeHandler.h"

void FileChangeHandler::registerCapabilities (Init::ServerCapabilities &capabilities) {
    if (!capabilities.textDocumentSync) capabilities.textDocumentSync = Init::TextDocumentSyncOptions {};

    capabilities.textDocumentSync->openClose = true;

    capabilities.textDocumentSync->change = TextDocumentSyncKind::Incremental;

    capabilities.textDocumentSync->willSave = true;

    capabilities.textDocumentSync->willSaveWaitUntil = false;
}

void FileChangeHandler::run (optional<Value> &params) {

}
