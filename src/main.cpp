#include <iostream>
#include <string>
#include "filesystem/FileManager.h"
#include "lconfig.h"

#include "handlerRegistration.h"
#include "MessageHandler.h"
#include "QueueManager.h"

#include "filesystem/UtfHandler.h"
#include <filesystem/File.h>
#include <biber/Style.h>

#include "text-buffer.h"


using std::u16string;

GlobalConfig *g_config;

int main (int argc, char** argv, const char** env) {
    std::cerr << "Running latex-language-server...\n";

    g_config = new GlobalConfig();

    auto *style = new Style::Style("/home/benjamin/github/latex-language-server/test/resources/project1/main.bcf");

    g_config->bibtex.style = style;

    FileManager::init();
    QueueManager::init();

    auto messageHandler = new MessageHandler();

    messageHandler->init();

    registerHandlers(messageHandler);

    messageHandler->run();

    return 0;
}
