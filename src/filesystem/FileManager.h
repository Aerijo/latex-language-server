#ifndef LATEX_LANGUAGE_SERVER_FILEMANAGER_H
#define LATEX_LANGUAGE_SERVER_FILEMANAGER_H

#include <unordered_map>

#include "Uri.h"
#include "File.h"

typedef string AbsolutePath;

using std::unordered_map;

class FileManager {
private:
    FileManager () = default;
    static FileManager *instance;

    static unordered_map<AbsolutePath, File*> files;

public:
    static void init ();
    static FileManager *getInstance () { return instance; };

    static void add (string &path, File *file);

    static void printFiles ();
};

#endif //LATEX_LANGUAGE_SERVER_FILEMANAGER_H
