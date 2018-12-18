//
// Created by Benjamin Gray on 18/12/18.
//

#include <optional>

#include <rapidjson/writer.h>

using std::optional;
using std::string;


/**
 * Well formed input is:
 * - A list of headers followed by the content
 *  - At least one header is required
 *  - Encoded in ASCII
 *  - Each is terminated by \r\n
 *  - The header section is terminated by a further \r\n
 * -
 *
 * @return
 */
optional<string> getJSONfromStdin () {
    bool exiting = false;

    string message;
    while (true) {
        int c = getchar();

        if (exiting) {
            if (c == '\n') {
                return message;
            } else {

            }
        } else if (c == '\r') {
            exiting = true;
        }
    }
};