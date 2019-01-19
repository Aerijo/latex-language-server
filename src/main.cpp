#include <iostream>
#include <string>

#include <locale>

//#include <utf8-conversions.h>

#include <tree_sitter/runtime.h>
#include <filesystem/Uri.h>

#include "filesystem/FileManager.h"
#include "lconfig.h"

#include "handlerRegistration.h"
#include "MessageHandler.h"
#include "QueueManager.h"

#include "text-buffer.h"

#include <encoding-conversion.h>

extern "C" { TSLanguage *tree_sitter_biber(); }

using std::u16string;

GlobalConfig *g_config;

int main (int argc, char** argv, const char** env) {
    std::cerr << "Running latex-language-server...\n";

    g_config = new GlobalConfig();

    TextBuffer buffer {};

    auto conversion = transcoding_to("UTF-8");
    u16string string = u"ab" "\xd83d" "\xde01" "cd";  // 'ab😁cd'

    size_t foo = 10;

    std::string output;

    std::cout << string.length() << std::endl;

    output.resize(foo);

    size_t bytes_encoded = 0, start = 0;

    bytes_encoded = conversion->encode(
            string, &start, string.size(), output.data(), output.size());

    output.resize(bytes_encoded);

    std::cout << output << std::endl;

    TSParser *parser = ts_parser_new();

    ts_parser_set_language(parser, tree_sitter_biber());

    FileManager::init();
    QueueManager::init();

    auto messageHandler = new MessageHandler();

    messageHandler->init();

    registerHandlers(messageHandler);

    messageHandler->run();

    return 0;
}
