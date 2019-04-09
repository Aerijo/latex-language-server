#include <algorithm>
#include <vector>
#include <string>
#include <cwctype>
#include <cstring>
#include <iostream>

#include "tree_sitter/parser.h"

namespace {

using std::vector;
using std::string;

enum TokenType {
  ERROR,
  VERBATIM,
  STAR,
  S_WHITESPACE, // allows newline
  M_WHITESPACE, // no newline (really is n mode)
};

struct Scanner {
  Scanner () {}

  void destroy () {}

  unsigned serialize (char *buffer) {
    int i = 9;
    return i;
  }

  void deserialize (const char *buffer, unsigned length) {}

  bool scan_verbatim (TSLexer *lexer, bool starValid) {
    // NOTE: ' ' (space) is a valid delim character
    //   As is '*'; the first star is gobbled by the main grammar if present
    char start_delim;
    switch (lexer->lookahead) {
      case '\n':
      case '\0':
        return false;
      default:
        start_delim = lexer->lookahead;
        lexer->advance(lexer, false);
    }

    if (starValid && start_delim == '*') return false;

    while (lexer->lookahead && lexer->lookahead != start_delim && lexer->lookahead != '\n') {
      lexer->advance(lexer, false);
    }

    if (lexer->lookahead == start_delim) lexer->advance(lexer, false);
    lexer->mark_end(lexer);

    lexer->result_symbol = VERBATIM;
    return true;
  }


  bool isErrorDetectionMode (const bool *validSymbols) {
    return validSymbols[ERROR];
  }

  bool is_inline_space (uint32_t symbol) {
    return symbol == ' ' || symbol == '\t';
  }

  bool scan_sm_whitespace (TSLexer *lexer, bool s_mode) {
    if (!(lexer->lookahead && is_inline_space(lexer->lookahead))) {
      if (s_mode && lexer->lookahead == '\n') {
        lexer->advance(lexer, false);
        lexer->result_symbol = S_WHITESPACE;
        return true;
      }
      return false;
    };
    lexer->advance(lexer, false);

    while (lexer->lookahead && is_inline_space(lexer->lookahead)) lexer->advance(lexer, false);

    lexer->result_symbol = M_WHITESPACE;
    if (lexer->lookahead == '\n') {
      if (s_mode) {
        lexer->advance(lexer, false);
        lexer->result_symbol = S_WHITESPACE;
      } else {
        return false;
      }
    }
    lexer->mark_end(lexer);
    return true;
  }

  bool scan (TSLexer *lexer, const bool *valid_symbols) {
    if (isErrorDetectionMode(valid_symbols)) {
      return false;
    }

    if (valid_symbols[STAR] && lexer->lookahead == '*') {
      lexer->advance(lexer, false);
      lexer->result_symbol = STAR;
      return true;
    }

    if (valid_symbols[M_WHITESPACE]) {
      if (scan_sm_whitespace(lexer, valid_symbols[S_WHITESPACE])) return true;
    }

    if (valid_symbols[VERBATIM]) {
      return scan_verbatim(lexer, valid_symbols[STAR]);
    }

    return false;
  }
};

}


extern "C" {

void *tree_sitter_latex_external_scanner_create() {
  return new Scanner();
}

bool tree_sitter_latex_external_scanner_scan(void *payload, TSLexer *lexer, const bool *valid_symbols) {
  Scanner *scanner = static_cast<Scanner *>(payload);
  return scanner->scan(lexer, valid_symbols);
}

unsigned tree_sitter_latex_external_scanner_serialize(void *payload, char *buffer) {
  Scanner *scanner = static_cast<Scanner *>(payload);
  return scanner->serialize(buffer);
}

void tree_sitter_latex_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
  Scanner *scanner = static_cast<Scanner *>(payload);
  scanner->deserialize(buffer, length);
}

void tree_sitter_latex_external_scanner_destroy(void *payload) {
  Scanner *scanner = static_cast<Scanner *>(payload);
  scanner->destroy();
  delete scanner;
}

}
