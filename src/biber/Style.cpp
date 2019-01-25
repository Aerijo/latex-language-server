#include <iostream>
#include "Style.h"

namespace Style {

/*
 * Generates a Style from a .bcf file (once I learn how it works -_-).
 *
 * The meaning of a .bcf file is described in the Biber docs,
 * which can be found most easily with `texdoc biber`.
 */
Style::Style (xml_document<> &doc) {
    std::cerr << "constructing Style from xml...\n";

    // NOTE: bcf:options component="biber" contains encoding information


}

}
