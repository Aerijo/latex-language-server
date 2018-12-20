cmake_minimum_required(VERSION 3.12)
project(latex_language_server)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(latex_language_server
        src/main.cpp
        src/parser.cpp
        src/parser.h src/texcount.cpp src/texcount.h src/options.cpp src/options.h src/communications.cpp src/message_handler.cpp src/message_handler.h src/message.cpp src/message.h src/TextDocument.cpp src/TextDocument.h)

target_include_directories(latex_language_server PRIVATE third_party/rapidjson/include)