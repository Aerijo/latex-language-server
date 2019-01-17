cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(latex_language_server LANGUAGES C CXX VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")

add_compile_definitions(RAPIDJSON_HAS_STDSTRING=1)

add_executable(latex_language_server
        src/main.cpp
        src/config.h

        src/lsp-tools/QueueManager.cpp
        src/lsp-tools/ioThreads.cpp
        src/lsp-tools/position.cpp
        src/lsp-tools/ThreadedQueue.h
        src/lsp-tools/Handler.h
        src/lsp-tools/definitions.h
        src/lsp-tools/MessageHandler.cpp
        src/lsp-tools/handlerRegistration.h
        src/lsp-tools/messaging.cpp
        src/lsp-tools/HandlerManager.cpp

        src/handlers/InitializeHandler.cpp

        third_party/tree-sitter/src/runtime/runtime.c
        third_party/tree-sitter-latex/src/catcode.cc
        third_party/tree-sitter-latex/src/scanner.cc
        third_party/tree-sitter-latex/src/parser.c src/lsp-tools/definitions.cpp src/handlers/file/FileChangeHandler.cpp src/handlers/file/FileChangeHandler.h src/handlers/hover/HoverHandler.cpp src/handlers/hover/HoverHandler.h src/handlers/ShutDownHandler.cpp src/handlers/ShutDownHandler.h src/handlers/autocomplete/CompletionProvider.cpp src/handlers/autocomplete/CompletionProvider.h)

target_include_directories(latex_language_server PRIVATE
        src
        src/lsp-tools)

target_include_directories(latex_language_server PRIVATE
        third_party/rapidjson/include
        third_party/tree-sitter/include
        third_party/tree-sitter/src
        third_party/tree-sitter/externals/utf8proc
        third_party/tree-sitter-latex/src)
