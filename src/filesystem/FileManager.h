#ifndef LATEX_LANGUAGE_SERVER_FILEMANAGER_H
#define LATEX_LANGUAGE_SERVER_FILEMANAGER_H

#include <unordered_map>

class FileManager {
private:
    FileManager () = default;
    static FileManager *instance;

public:
    static void init ();
    static FileManager *getInstance () { return instance; };


};

#endif //LATEX_LANGUAGE_SERVER_FILEMANAGER_H
