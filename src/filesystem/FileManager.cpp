#include "FileManager.h"

FileManager *FileManager::instance {};

unordered_map<string, File *> FileManager::files {};

void FileManager::init () {
    delete instance;
    instance = new FileManager();
}

void FileManager::printFiles () {
    for (auto it : files) {
        std::cerr << "file:" << it.first << "\n";
        it.second->print(std::cerr);
        std::cerr << std::endl;
    }
}

void FileManager::add (string &path, File *file) {
    files[path] = file;
}
