cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(latex_language_server LANGUAGES C CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17 -Wall")

add_compile_definitions(RAPIDJSON_HAS_STDSTRING=1)

add_compile_definitions(HAVE_CONFIG_H)
add_compile_definitions(PCRE2_CODE_UNIT_WIDTH=16)
add_compile_definitions(SUPPORT_JIT)

add_executable(latex_language_server
        src/main.cpp
        src/lconfig.h

        src/lsp-tools/QueueManager.cpp
        src/lsp-tools/ThreadedQueue.h
        src/lsp-tools/MessageHandler.cpp
        src/lsp-tools/messaging.cpp
        src/lsp-tools/HandlerManager.cpp

        src/handlers/InitializeHandler.cpp

        third_party/tree-sitter/src/runtime/runtime.c

        third_party/tree-sitter-biber/src/parser.c

        third_party/tree-sitter-latex/src/parser.c
        third_party/tree-sitter-latex/src/scanner.cc

        third_party/superstring/vendor/pcre2-10.32/src/pcre2_auto_possess.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_chartables.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_compile.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_config.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_context.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_convert.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_dfa_match.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_error.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_extuni.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_find_bracket.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_jit_compile.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_maketables.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_match.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_match_data.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_newline.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_ord2utf.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_pattern_info.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_serialize.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_string_utils.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_study.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_substitute.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_substring.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_tables.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_ucd.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_valid_utf.c
        third_party/superstring/vendor/pcre2-10.32/src/pcre2_xclass.c

        third_party/superstring/src/core/encoding-conversion.cc
        third_party/superstring/src/core/regex.cc
        third_party/superstring/src/core/marker-index.cc
        third_party/superstring/src/core/range.cc
        third_party/superstring/src/core/patch.cc
        third_party/superstring/src/core/point.cc
        third_party/superstring/src/core/text-slice.cc
        third_party/superstring/src/core/text-slice.cc
        third_party/superstring/src/core/text.cc
        third_party/superstring/src/core/text-buffer.cc

        src/lsp-tools/definitions.cpp
        src/handlers/file/FileChangeHandler.cpp
        src/handlers/hover/HoverHandler.cpp
        src/handlers/ShutDownHandler.cpp
        src/handlers/autocomplete/CompletionProvider.cpp
        src/filesystem/FileManager.cpp
        src/handlers/file/FileOpenHandler.cpp
        src/handlers/file/FileCloseHandler.cpp
        src/handlers/file/FileSaveHandler.cpp
        src/filesystem/File.cpp
        src/filesystem/Uri.cpp
        src/filesystem/UtfHandler.cpp
        src/filesystem/TextBufferInput.cpp
        src/biber/BibManager.cpp
        src/biber/BibIndexer.cpp
        src/biber/Style.cpp
        src/lsp-tools/utils.cpp
        src/lsp-tools/utils.h
        src/handlers/outline/OutlineHandler.cpp
        src/handlers/outline/OutlineHandler.h
        src/handlers/util.cpp
        src/handlers/util.h)

target_include_directories(latex_language_server PRIVATE
        src)

target_include_directories(latex_language_server PRIVATE
        third_party/rapidjson/include
        third_party/rapidxml-1.13
        third_party/tree-sitter/include
        third_party/tree-sitter/src
        third_party/tree-sitter/externals/utf8proc
        third_party/tree-sitter-biber/src
        third_party/tree-sitter-latex/src
        third_party/superstring/src/core
        third_party/superstring/vendor/libcxx
        third_party/superstring/vendor/pcre2-10.32/src)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()
