#include "FileManager.h"

FileManager *FileManager::instance {};

void FileManager::init () {
    delete instance;
    instance = new FileManager();
}
