#include <iostream>
#include <fstream>
#include <string>
#include "filesystem/FileManager.h"
#include "lconfig.h"

#include "lsp-tools/handlerRegistration.h"
#include "lsp-tools/MessageHandler.h"
#include "lsp-tools/QueueManager.h"

#include "filesystem/UtfHandler.h"
#include <filesystem/File.h>
#include <biber/Style.h>

#include "text-buffer.h"

#include "./cwl/parseCWL.cpp"

using std::u16string;

GlobalConfig *g_config;

int main (int argc, char** argv, const char** env) {
    std::cerr << "Running latex-language-server...\n";

    g_config = new GlobalConfig();

    FileManager::init();
    QueueManager::init();



    parseCWL("/home/benjamin/github/latex-language-server/src/cwl/graphicx.cwl");

    

    auto messageHandler = new MessageHandler();

    messageHandler->init();

    registerHandlers(messageHandler);

    messageHandler->run();

    return 0;
}
