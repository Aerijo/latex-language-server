#include <iostream>

#include "ioThreads.h"

int main () {
    std::cout << "Running..." << std::endl;

    launchStdinLoop().join();
    launchStdoutLoop().join();
}