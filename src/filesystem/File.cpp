#include "File.h"

bool File::operator== (File &that) {
    return uri == that.uri;
}
