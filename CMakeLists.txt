cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(latex_language_server LANGUAGES C CXX VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(latex_language_server
        src/main.cpp
        src/lsp-tools/QueueManager.cpp
        src/lsp-tools/ioThreads.cpp
        src/lsp-tools/library.cpp
        src/lsp-tools/position.cpp
        src/lsp-tools/ThreadedQueue.cpp
        third_party/tree-sitter/src/runtime/runtime.c)

target_include_directories(latex_language_server PRIVATE
        src
        src/lsp-tools)

target_include_directories(latex_language_server PRIVATE
        third_party/rapidjson/include
        third_party/tree-sitter/include
        third_party/tree-sitter/src
        third_party/tree-sitter/externals/utf8proc
        third_party/tree-sitter-latex/src
        )
