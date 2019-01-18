#include <iostream>
#include "FileOpenHandler.h"

void FileOpenHandler::run (optional<Value> &params) {
    if (!params) { handleMissingFileOpenParams(); return; }

//    Value &value = params.value();
//
//    Value &textDocument = value["textDocument"];
//
//    string uri = textDocument["uri"].GetString();
//
//    string languageId = textDocument["languageId"].GetString();
//
//    uint_fast64_t version = textDocument["version"].GetUint64();
//
//    string text = textDocument["text"].GetString();


}

void FileOpenHandler::handleMissingFileOpenParams () {
    std::cerr << "Missing file open params!! Possibly out of sync!!\n";

    // TODO: Request resync when this happens
}
