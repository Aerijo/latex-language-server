#include "communications.h"
#include "message_handler.h"

int main (int argc, char** argv, const char** env) {

    Document initRequest = awaitAndGetStdinMessage();

    handleInitRequest(initRequest);

    while (true) {
        Document input = awaitAndGetStdinMessage();

        if (input.HasMember("foo")) break;

        handleMessage(input);
    }

    return 0;
}
