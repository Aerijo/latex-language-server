#include <iostream>
#include <string>

#include <locale>

#include <tree_sitter/runtime.h>
#include <filesystem/Uri.h>

#include "filesystem/FileManager.h"
#include "lconfig.h"

#include "handlerRegistration.h"
#include "MessageHandler.h"
#include "QueueManager.h"

#include "filesystem/UtfHandler.h"
#include <filesystem/File.h>

#include "text-buffer.h"

#include <rapidxml.hpp>
#include <fstream>
#include <biber/Style.h>

extern "C" { TSLanguage *tree_sitter_biber(); }

using std::u16string;

GlobalConfig *g_config;

int main (int argc, char** argv, const char** env) {
    std::cerr << "Running latex-language-server...\n";

    std::cerr << sizeof(unordered_map<u16string, Style::Entry>) << "\n";

    g_config = new GlobalConfig();

    FileManager::init();
    QueueManager::init();

    std::ifstream infile { "../test/resources/project1/main.bcf" };
    string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };

    std::cerr << "content length: " << file_contents.length() << "\n";

    rapidxml::xml_document<> doc;
    char* cstr = new char[file_contents.size() + 1];  // Create char buffer to store string copy
    strcpy (cstr, file_contents.c_str());
    doc.parse<0>(cstr);

    auto style = Style::Style(doc);

    delete [] cstr;

    std::cerr << "constructed\n";



    auto messageHandler = new MessageHandler();

    messageHandler->init();

    registerHandlers(messageHandler);

    messageHandler->run();

    return 0;
}
