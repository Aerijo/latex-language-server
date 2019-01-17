#ifndef LATEX_LANGUAGE_SERVER_FILE_H
#define LATEX_LANGUAGE_SERVER_FILE_H

#include "Uri.h"

class File {
    File (Uri &uri) : uri (uri) {}

    Uri uri;

};

#endif //LATEX_LANGUAGE_SERVER_FILE_H
