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
  CONTROL_SYMBOL,
  CONTROL_WORD,
  BEGIN_ENV,
  END_ENV,
  INLINE_MATH_START,
  INLINE_MATH_END,
  DISPLAY_MATH_START,
  DISPLAY_MATH_END,
  MATH_SHIFT_ERROR,
};

struct Scanner {
  Scanner () = default;

  void destroy () {}

  unsigned serialize (char *buffer) {
    return 0;
  }

  void deserialize (const char *buffer, unsigned length) {}

  bool is_error_mode (const bool *validSymbols) {
    return validSymbols[ERROR];
  }

  bool is_inline_space (int32_t symbol) {
    return symbol == ' ' || symbol == '\t';
  }

  /**
   * This is checked immediately following a command that is expected to
   * take an argument. The whitespace rules are
   * 1) newline permitted on end of the first line
   * 2) newline not permitted on any subsequent lines
   *
   * Note comments "hide" the newline, so it is possible to have multiple
   * "blank" lines that end with comments, and still have the next group
   * be an argument.
   *
   * This function does not advance the end of the lexer. It only serves
   * to identify if the control sequence has a param.
   */
  bool command_has_arg (TSLexer *lexer) {
    while (lexer->lookahead && is_inline_space(lexer->lookahead)) {
      lexer->advance(lexer, true);
    }

    switch (lexer->lookahead) {
      case '\n':
        break;
      case '%':
        while (lexer->lookahead && lexer->lookahead != '\n') {
          lexer->advance(lexer, true);
        }
        break;
      default:
        return lexer->lookahead == '{';
    }

    while (lexer->lookahead) {
      lexer->advance(lexer, true);

      while (lexer->lookahead && is_inline_space(lexer->lookahead)) {
        lexer->advance(lexer, true);
      }

      if (lexer->lookahead != '%') {
        return lexer->lookahead == '{';
      }

      while (lexer->lookahead && lexer->lookahead != '\n') {
        lexer->advance(lexer, true);
      }
    }

    return true;
  }

  bool scan_env_command (TSLexer *lexer, TokenType type, const char *name, int length) {
    int i = 0;
    do {
      lexer->advance(lexer, false);
      if (lexer->lookahead != name[i]) {
        return scan_generic_control_sequence(lexer);
      }
      i++;
    } while (i < length);

    lexer->advance(lexer, false);
    lexer->mark_end(lexer);

    if (command_has_arg(lexer)) {
      lexer->result_symbol = type;
    } else {
      return scan_generic_control_sequence(lexer);
    }

    return true;
  }

  bool scan_generic_control_sequence (TSLexer *lexer) {
    do {
      lexer->advance(lexer, false);
    } while (isalpha(lexer->lookahead));
    lexer->mark_end(lexer);
    lexer->result_symbol = CONTROL_WORD;
    return true;
  }

  bool scan_verb_command (TSLexer *lexer) {
    const char name[] = "erb";
    int i = 0;
    do {
      lexer->advance(lexer, false);
      if (lexer->lookahead != name[i]) {
        return scan_generic_control_sequence(lexer);
      }
      i++;
    } while (i < 3);

    lexer->advance(lexer, false);
    if (isalnum(lexer->lookahead)) {
      return scan_generic_control_sequence(lexer);
    }

    lexer->result_symbol = VERBATIM;

    // \verb allows a starred variant
    if (lexer->lookahead == '*') { lexer->advance(lexer, false); }

    int32_t start_delim;
    switch (lexer->lookahead) {
      case '\n':
      case '\0':
        lexer->mark_end(lexer);
        return true;
      default:
        start_delim = lexer->lookahead;
        lexer->advance(lexer, false);
    }

    while (lexer->lookahead && lexer->lookahead != start_delim && lexer->lookahead != '\n') {
      lexer->advance(lexer, false);
    }

    if (lexer->lookahead == start_delim) {
      lexer->advance(lexer, false);
    }

    lexer->mark_end(lexer);
    return true;
  }

  bool scan_control_sequence (TSLexer *lexer, const bool *valid_symbols) {
    if (lexer->lookahead != '\\') { return false; }
    lexer->advance(lexer, true); // skip the backslash

    char c = lexer->lookahead;
    if (!isalpha(c)) {
      lexer->advance(lexer, false);
      lexer->mark_end(lexer);

      /*
       * We cannot start a different kind of maths inside
       * an existing math environment. Nor can we close
       * math without a matching opener. We mark all detected
       * cases of this with MATH_SHIFT_ERROR, so the parsing
       * does not break.
       */
      if (c == '(') {
        lexer->result_symbol = valid_symbols[DISPLAY_MATH_END]
          ? MATH_SHIFT_ERROR
          : INLINE_MATH_START;
      } else if (c == '[') {
        lexer->result_symbol = valid_symbols[INLINE_MATH_END]
          ? MATH_SHIFT_ERROR
          : DISPLAY_MATH_START;
      } else if (c == ')') {
        lexer->result_symbol = valid_symbols[INLINE_MATH_END]
          ? INLINE_MATH_END
          : MATH_SHIFT_ERROR;
      } else if (c == ']') {
        lexer->result_symbol = valid_symbols[DISPLAY_MATH_END]
          ? DISPLAY_MATH_END
          : MATH_SHIFT_ERROR;
      } else {
        lexer->result_symbol = CONTROL_SYMBOL;
      }

      return true;
    }

    if (c == 'b') {
      const char name[] { 'e', 'g', 'i', 'n' };
      return scan_env_command(lexer, BEGIN_ENV, name, 4);
    } else if (c == 'e' && valid_symbols[END_ENV]) {
      const char name[] { 'n', 'd' };
      return scan_env_command(lexer, END_ENV, name, 2);
    } else if (c == 'v') {
      return scan_verb_command(lexer);
    }

    while (lexer->lookahead && isalpha(lexer->lookahead)) {
      lexer->advance(lexer, false);
    }

    lexer->mark_end(lexer);
    lexer->result_symbol = CONTROL_WORD;
    return true;
  }

  bool scan_math_shift (TSLexer *lexer, const bool *valid_symbols) {
    // lookahead == '$'
    lexer->advance(lexer, false);

    if (valid_symbols[INLINE_MATH_END]) {
      lexer->mark_end(lexer);
      lexer->result_symbol = INLINE_MATH_END;
      return true;
    }

    if (lexer->lookahead != '$') {
      lexer->mark_end(lexer);

      // a single $ is an error in display math mode
      lexer->result_symbol = valid_symbols[DISPLAY_MATH_END]
        ? MATH_SHIFT_ERROR
        : INLINE_MATH_START;

      return true;
    }

    lexer->advance(lexer, false);
    lexer->mark_end(lexer);
    lexer->result_symbol = valid_symbols[DISPLAY_MATH_END]
      ? DISPLAY_MATH_END
      : DISPLAY_MATH_START;

    return true;
  }

  bool scan (TSLexer *lexer, const bool *valid_symbols) {
    // if (is_error_mode(valid_symbols)) {
    //   std::cerr << "ERROR!!!\n";
    // }
    const int32_t c = lexer->lookahead;

    if (c == '\\' && valid_symbols[CONTROL_WORD]) {
      return scan_control_sequence(lexer, valid_symbols);
    }

    if (c == '$' && (
        valid_symbols[INLINE_MATH_START] ||
        valid_symbols[INLINE_MATH_END] ||
        valid_symbols[DISPLAY_MATH_START] ||
        valid_symbols[DISPLAY_MATH_END]
      )) {
      return scan_math_shift(lexer, valid_symbols);
    }

    if (c == '}' && (valid_symbols[INLINE_MATH_END] || valid_symbols[DISPLAY_MATH_END])) {
      // implicit mathmode end
    }

    // also tries this if in error mode
    if (valid_symbols[CONTROL_WORD]) { // CONTROL_SYMBOL will also always be valid
      return scan_control_sequence(lexer, valid_symbols);
    }

    if (valid_symbols[STAR] && c == '*') {
      lexer->advance(lexer, false);
      lexer->result_symbol = STAR;
      return true;
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
