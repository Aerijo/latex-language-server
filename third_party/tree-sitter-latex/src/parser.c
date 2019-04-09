#include <tree_sitter/parser.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 9
#define STATE_COUNT 44
#define SYMBOL_COUNT 35
#define ALIAS_COUNT 0
#define TOKEN_COUNT 18
#define EXTERNAL_TOKEN_COUNT 5
#define MAX_ALIAS_SEQUENCE_LENGTH 4

enum {
  sym__escape_char = 1,
  sym_begin_group = 2,
  sym_end_group = 3,
  anon_sym_DOLLAR = 4,
  sym_display_math_shift = 5,
  sym_text = 6,
  sym_symbol = 7,
  sym_letters = 8,
  sym_comment = 9,
  anon_sym_verb = 10,
  anon_sym_begin = 11,
  anon_sym_end = 12,
  sym__error = 13,
  sym_verb = 14,
  sym_star = 15,
  sym_s_whitespace = 16,
  sym_m_whitespace = 17,
  sym_program = 18,
  sym_inline_math_shift = 19,
  sym__text_mode = 20,
  sym__simple_group = 21,
  sym_text_group = 22,
  sym__control_sequence = 23,
  sym_control_symbol = 24,
  sym_control_word = 25,
  sym_verbatim = 26,
  sym_environment = 27,
  sym_env_body = 28,
  sym_open_env = 29,
  sym_begin_command = 30,
  sym_close_env = 31,
  sym_end_command = 32,
  aux_sym__text_mode_repeat1 = 33,
  aux_sym__simple_group_repeat1 = 34,
};

static const char *ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [sym__escape_char] = "_escape_char",
  [sym_begin_group] = "begin_group",
  [sym_end_group] = "end_group",
  [anon_sym_DOLLAR] = "$",
  [sym_display_math_shift] = "display_math_shift",
  [sym_text] = "text",
  [sym_symbol] = "cs",
  [sym_letters] = "cs",
  [sym_comment] = "comment",
  [anon_sym_verb] = "cs",
  [anon_sym_begin] = "cs",
  [anon_sym_end] = "cs",
  [sym__error] = "_error",
  [sym_verb] = "verb",
  [sym_star] = "star",
  [sym_s_whitespace] = "s_whitespace",
  [sym_m_whitespace] = "m_whitespace",
  [sym_program] = "program",
  [sym_inline_math_shift] = "inline_math_shift",
  [sym__text_mode] = "_text_mode",
  [sym__simple_group] = "env_name",
  [sym_text_group] = "text_group",
  [sym__control_sequence] = "_control_sequence",
  [sym_control_symbol] = "control_symbol",
  [sym_control_word] = "control_word",
  [sym_verbatim] = "verbatim",
  [sym_environment] = "environment",
  [sym_env_body] = "env_body",
  [sym_open_env] = "open_env",
  [sym_begin_command] = "begin_command",
  [sym_close_env] = "close_env",
  [sym_end_command] = "end_command",
  [aux_sym__text_mode_repeat1] = "_text_mode_repeat1",
  [aux_sym__simple_group_repeat1] = "_simple_group_repeat1",
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [sym__escape_char] = {
    .visible = false,
    .named = true,
  },
  [sym_begin_group] = {
    .visible = true,
    .named = true,
  },
  [sym_end_group] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_DOLLAR] = {
    .visible = true,
    .named = false,
  },
  [sym_display_math_shift] = {
    .visible = true,
    .named = true,
  },
  [sym_text] = {
    .visible = true,
    .named = true,
  },
  [sym_symbol] = {
    .visible = true,
    .named = true,
  },
  [sym_letters] = {
    .visible = true,
    .named = true,
  },
  [sym_comment] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_verb] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_begin] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_end] = {
    .visible = true,
    .named = true,
  },
  [sym__error] = {
    .visible = false,
    .named = true,
  },
  [sym_verb] = {
    .visible = true,
    .named = true,
  },
  [sym_star] = {
    .visible = true,
    .named = true,
  },
  [sym_s_whitespace] = {
    .visible = true,
    .named = true,
  },
  [sym_m_whitespace] = {
    .visible = true,
    .named = true,
  },
  [sym_program] = {
    .visible = true,
    .named = true,
  },
  [sym_inline_math_shift] = {
    .visible = true,
    .named = true,
  },
  [sym__text_mode] = {
    .visible = false,
    .named = true,
  },
  [sym__simple_group] = {
    .visible = true,
    .named = true,
  },
  [sym_text_group] = {
    .visible = true,
    .named = true,
  },
  [sym__control_sequence] = {
    .visible = false,
    .named = true,
  },
  [sym_control_symbol] = {
    .visible = true,
    .named = true,
  },
  [sym_control_word] = {
    .visible = true,
    .named = true,
  },
  [sym_verbatim] = {
    .visible = true,
    .named = true,
  },
  [sym_environment] = {
    .visible = true,
    .named = true,
  },
  [sym_env_body] = {
    .visible = true,
    .named = true,
  },
  [sym_open_env] = {
    .visible = true,
    .named = true,
  },
  [sym_begin_command] = {
    .visible = true,
    .named = true,
  },
  [sym_close_env] = {
    .visible = true,
    .named = true,
  },
  [sym_end_command] = {
    .visible = true,
    .named = true,
  },
  [aux_sym__text_mode_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym__simple_group_repeat1] = {
    .visible = false,
    .named = false,
  },
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  switch (state) {
    case 0:
      if (lookahead == 0)
        ADVANCE(1);
      if (lookahead == '$')
        ADVANCE(2);
      if (lookahead == '%')
        ADVANCE(3);
      if (lookahead == '\\')
        ADVANCE(4);
      if (lookahead == 'b')
        ADVANCE(5);
      if (lookahead == 'e')
        ADVANCE(6);
      if (lookahead == 'v')
        ADVANCE(7);
      if (lookahead == '{')
        ADVANCE(8);
      if (lookahead == '}')
        ADVANCE(9);
      if (('#' <= lookahead && lookahead <= '&') ||
          lookahead == '^' ||
          lookahead == '_' ||
          lookahead == '~')
        ADVANCE(10);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0)
        ADVANCE(12);
      END_STATE();
    case 1:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 2:
      ACCEPT_TOKEN(anon_sym_DOLLAR);
      END_STATE();
    case 3:
      ACCEPT_TOKEN(sym_symbol);
      if (lookahead == '\n')
        ADVANCE(13);
      if (lookahead != 0)
        ADVANCE(14);
      END_STATE();
    case 4:
      ACCEPT_TOKEN(sym__escape_char);
      END_STATE();
    case 5:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == 'e')
        ADVANCE(15);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 6:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == 'n')
        ADVANCE(17);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 7:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == 'e')
        ADVANCE(18);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 8:
      ACCEPT_TOKEN(sym_begin_group);
      END_STATE();
    case 9:
      ACCEPT_TOKEN(sym_end_group);
      END_STATE();
    case 10:
      ACCEPT_TOKEN(sym_symbol);
      END_STATE();
    case 11:
      ACCEPT_TOKEN(sym_text);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 12:
      ACCEPT_TOKEN(sym_text);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 13:
      ACCEPT_TOKEN(sym_comment);
      END_STATE();
    case 14:
      ACCEPT_TOKEN(sym_comment);
      if (lookahead == '\n')
        ADVANCE(13);
      if (lookahead != 0)
        ADVANCE(14);
      END_STATE();
    case 15:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == 'g')
        ADVANCE(19);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 16:
      ACCEPT_TOKEN(sym_text);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 17:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == 'd')
        ADVANCE(20);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 18:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == 'r')
        ADVANCE(21);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 19:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == 'i')
        ADVANCE(22);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(anon_sym_end);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 21:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == 'b')
        ADVANCE(23);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == 'n')
        ADVANCE(24);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 23:
      ACCEPT_TOKEN(anon_sym_verb);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 24:
      ACCEPT_TOKEN(anon_sym_begin);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(11);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 25:
      if (lookahead == 0)
        ADVANCE(1);
      if (lookahead == '%')
        ADVANCE(14);
      if (lookahead == '\\')
        ADVANCE(4);
      if (lookahead == '{')
        ADVANCE(8);
      if (lookahead == '}')
        ADVANCE(9);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 26:
      if (lookahead == '%')
        ADVANCE(3);
      if (lookahead == 'b')
        ADVANCE(27);
      if (lookahead == 'v')
        ADVANCE(28);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      if (lookahead != 0)
        ADVANCE(10);
      END_STATE();
    case 27:
      ACCEPT_TOKEN(sym_letters);
      if (lookahead == 'e')
        ADVANCE(30);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 28:
      ACCEPT_TOKEN(sym_letters);
      if (lookahead == 'e')
        ADVANCE(31);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 29:
      ACCEPT_TOKEN(sym_letters);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 30:
      ACCEPT_TOKEN(sym_letters);
      if (lookahead == 'g')
        ADVANCE(32);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 31:
      ACCEPT_TOKEN(sym_letters);
      if (lookahead == 'r')
        ADVANCE(33);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 32:
      ACCEPT_TOKEN(sym_letters);
      if (lookahead == 'i')
        ADVANCE(34);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 33:
      ACCEPT_TOKEN(sym_letters);
      if (lookahead == 'b')
        ADVANCE(35);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 34:
      ACCEPT_TOKEN(sym_letters);
      if (lookahead == 'n')
        ADVANCE(36);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 35:
      ACCEPT_TOKEN(anon_sym_verb);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 36:
      ACCEPT_TOKEN(anon_sym_begin);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 37:
      if (lookahead == 0)
        ADVANCE(1);
      if (lookahead == '$')
        ADVANCE(38);
      if (lookahead == '%')
        ADVANCE(14);
      if (lookahead == '\\')
        ADVANCE(4);
      if (lookahead == '{')
        ADVANCE(8);
      if (lookahead == '}')
        ADVANCE(9);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 38:
      ACCEPT_TOKEN(anon_sym_DOLLAR);
      if (lookahead == '$')
        ADVANCE(39);
      END_STATE();
    case 39:
      ACCEPT_TOKEN(sym_display_math_shift);
      END_STATE();
    case 40:
      if (lookahead == '%')
        ADVANCE(3);
      if (lookahead == 'b')
        ADVANCE(27);
      if (lookahead == 'e')
        ADVANCE(41);
      if (lookahead == 'v')
        ADVANCE(28);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      if (lookahead != 0)
        ADVANCE(10);
      END_STATE();
    case 41:
      ACCEPT_TOKEN(sym_letters);
      if (lookahead == 'n')
        ADVANCE(42);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 42:
      ACCEPT_TOKEN(sym_letters);
      if (lookahead == 'd')
        ADVANCE(43);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 43:
      ACCEPT_TOKEN(anon_sym_end);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      END_STATE();
    case 44:
      if (lookahead == '$')
        ADVANCE(38);
      if (lookahead == '%')
        ADVANCE(14);
      if (lookahead == '\\')
        ADVANCE(4);
      if (lookahead == '}')
        ADVANCE(9);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '~')
        ADVANCE(16);
      END_STATE();
    case 45:
      if (lookahead == '%')
        ADVANCE(14);
      if (lookahead == 'e')
        ADVANCE(46);
      END_STATE();
    case 46:
      if (lookahead == 'n')
        ADVANCE(47);
      END_STATE();
    case 47:
      if (lookahead == 'd')
        ADVANCE(48);
      END_STATE();
    case 48:
      ACCEPT_TOKEN(anon_sym_end);
      END_STATE();
    case 49:
      if (lookahead == '%')
        ADVANCE(3);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(29);
      if (lookahead != 0)
        ADVANCE(10);
      END_STATE();
    default:
      return false;
  }
}

static TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0, .external_lex_state = 1},
  [1] = {.lex_state = 25},
  [2] = {.lex_state = 26},
  [3] = {.lex_state = 25},
  [4] = {.lex_state = 25},
  [5] = {.lex_state = 25},
  [6] = {.lex_state = 25},
  [7] = {.lex_state = 25},
  [8] = {.lex_state = 25},
  [9] = {.lex_state = 37},
  [10] = {.lex_state = 25},
  [11] = {.lex_state = 25, .external_lex_state = 2},
  [12] = {.lex_state = 37},
  [13] = {.lex_state = 25},
  [14] = {.lex_state = 25},
  [15] = {.lex_state = 25},
  [16] = {.lex_state = 25},
  [17] = {.lex_state = 40},
  [18] = {.lex_state = 25},
  [19] = {.lex_state = 25},
  [20] = {.lex_state = 25},
  [21] = {.lex_state = 25},
  [22] = {.lex_state = 25},
  [23] = {.lex_state = 44},
  [24] = {.lex_state = 25},
  [25] = {.lex_state = 25, .external_lex_state = 3},
  [26] = {.lex_state = 25},
  [27] = {.lex_state = 25},
  [28] = {.lex_state = 25},
  [29] = {.lex_state = 44},
  [30] = {.lex_state = 25},
  [31] = {.lex_state = 45},
  [32] = {.lex_state = 25},
  [33] = {.lex_state = 44},
  [34] = {.lex_state = 49},
  [35] = {.lex_state = 25},
  [36] = {.lex_state = 25},
  [37] = {.lex_state = 44},
  [38] = {.lex_state = 25},
  [39] = {.lex_state = 25},
  [40] = {.lex_state = 25},
  [41] = {.lex_state = 25},
  [42] = {.lex_state = 44},
  [43] = {.lex_state = 25},
};

enum {
  ts_external_token__error = 0,
  ts_external_token_verb = 1,
  ts_external_token_star = 2,
  ts_external_token_s_whitespace = 3,
  ts_external_token_m_whitespace = 4,
};

static TSSymbol ts_external_scanner_symbol_map[EXTERNAL_TOKEN_COUNT] = {
  [ts_external_token__error] = sym__error,
  [ts_external_token_verb] = sym_verb,
  [ts_external_token_star] = sym_star,
  [ts_external_token_s_whitespace] = sym_s_whitespace,
  [ts_external_token_m_whitespace] = sym_m_whitespace,
};

static bool ts_external_scanner_states[4][EXTERNAL_TOKEN_COUNT] = {
  [1] = {
    [ts_external_token__error] = true,
    [ts_external_token_star] = true,
    [ts_external_token_m_whitespace] = true,
    [ts_external_token_verb] = true,
    [ts_external_token_s_whitespace] = true,
  },
  [2] = {
    [ts_external_token_verb] = true,
    [ts_external_token_star] = true,
  },
  [3] = {
    [ts_external_token_verb] = true,
  },
};

static uint16_t ts_parse_table[STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [sym__error] = ACTIONS(1),
    [sym_comment] = ACTIONS(3),
    [anon_sym_DOLLAR] = ACTIONS(1),
    [sym__escape_char] = ACTIONS(1),
    [sym_text] = ACTIONS(1),
    [sym_star] = ACTIONS(1),
    [anon_sym_begin] = ACTIONS(1),
    [sym_end_group] = ACTIONS(1),
    [sym_letters] = ACTIONS(1),
    [ts_builtin_sym_end] = ACTIONS(1),
    [sym_m_whitespace] = ACTIONS(1),
    [sym_verb] = ACTIONS(1),
    [anon_sym_verb] = ACTIONS(1),
    [anon_sym_end] = ACTIONS(1),
    [sym_begin_group] = ACTIONS(1),
    [sym_symbol] = ACTIONS(1),
    [sym_s_whitespace] = ACTIONS(1),
  },
  [1] = {
    [sym__text_mode] = STATE(4),
    [sym_verbatim] = STATE(5),
    [sym_environment] = STATE(5),
    [sym_program] = STATE(6),
    [aux_sym__text_mode_repeat1] = STATE(5),
    [sym__control_sequence] = STATE(5),
    [sym_text_group] = STATE(5),
    [sym_open_env] = STATE(7),
    [sym_control_symbol] = STATE(5),
    [sym_control_word] = STATE(5),
    [sym_begin_command] = STATE(8),
    [sym_begin_group] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(7),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(11),
    [sym_text] = ACTIONS(13),
  },
  [2] = {
    [sym_comment] = ACTIONS(3),
    [sym_symbol] = ACTIONS(15),
    [anon_sym_begin] = ACTIONS(17),
    [anon_sym_verb] = ACTIONS(19),
    [sym_letters] = ACTIONS(21),
  },
  [3] = {
    [sym__text_mode] = STATE(14),
    [sym_verbatim] = STATE(15),
    [sym_environment] = STATE(15),
    [aux_sym__text_mode_repeat1] = STATE(15),
    [sym_text_group] = STATE(15),
    [sym__control_sequence] = STATE(15),
    [sym_open_env] = STATE(7),
    [sym_control_symbol] = STATE(15),
    [sym_control_word] = STATE(15),
    [sym_begin_command] = STATE(8),
    [sym_begin_group] = ACTIONS(5),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(11),
    [sym_text] = ACTIONS(23),
    [sym_end_group] = ACTIONS(25),
  },
  [4] = {
    [sym_comment] = ACTIONS(9),
    [ts_builtin_sym_end] = ACTIONS(27),
  },
  [5] = {
    [sym_verbatim] = STATE(16),
    [sym_environment] = STATE(16),
    [aux_sym__text_mode_repeat1] = STATE(16),
    [sym_text_group] = STATE(16),
    [sym__control_sequence] = STATE(16),
    [sym_open_env] = STATE(7),
    [sym_control_symbol] = STATE(16),
    [sym_control_word] = STATE(16),
    [sym_begin_command] = STATE(8),
    [sym_begin_group] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(29),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(11),
    [sym_text] = ACTIONS(31),
  },
  [6] = {
    [sym_comment] = ACTIONS(9),
    [ts_builtin_sym_end] = ACTIONS(33),
  },
  [7] = {
    [sym_close_env] = STATE(18),
    [sym__text_mode] = STATE(19),
    [sym_verbatim] = STATE(21),
    [sym_environment] = STATE(21),
    [sym_end_command] = STATE(20),
    [aux_sym__text_mode_repeat1] = STATE(21),
    [sym_text_group] = STATE(21),
    [sym__control_sequence] = STATE(21),
    [sym_env_body] = STATE(22),
    [sym_open_env] = STATE(7),
    [sym_control_symbol] = STATE(21),
    [sym_control_word] = STATE(21),
    [sym_begin_command] = STATE(8),
    [sym_begin_group] = ACTIONS(5),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(35),
    [sym_text] = ACTIONS(37),
  },
  [8] = {
    [sym_begin_group] = ACTIONS(39),
    [sym_comment] = ACTIONS(9),
  },
  [9] = {
    [sym_display_math_shift] = ACTIONS(41),
    [sym_comment] = ACTIONS(9),
    [ts_builtin_sym_end] = ACTIONS(41),
    [anon_sym_DOLLAR] = ACTIONS(43),
    [sym__escape_char] = ACTIONS(41),
    [sym_text] = ACTIONS(41),
    [sym_begin_group] = ACTIONS(41),
    [sym_end_group] = ACTIONS(41),
  },
  [10] = {
    [sym_begin_group] = ACTIONS(45),
    [sym_comment] = ACTIONS(9),
  },
  [11] = {
    [sym_verb] = ACTIONS(47),
    [sym_star] = ACTIONS(49),
    [sym_comment] = ACTIONS(9),
  },
  [12] = {
    [sym_display_math_shift] = ACTIONS(51),
    [sym_comment] = ACTIONS(9),
    [ts_builtin_sym_end] = ACTIONS(51),
    [anon_sym_DOLLAR] = ACTIONS(53),
    [sym__escape_char] = ACTIONS(51),
    [sym_text] = ACTIONS(51),
    [sym_begin_group] = ACTIONS(51),
    [sym_end_group] = ACTIONS(51),
  },
  [13] = {
    [sym_begin_group] = ACTIONS(55),
    [ts_builtin_sym_end] = ACTIONS(55),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(55),
    [sym_text] = ACTIONS(55),
    [sym_end_group] = ACTIONS(55),
  },
  [14] = {
    [sym_comment] = ACTIONS(9),
    [sym_end_group] = ACTIONS(57),
  },
  [15] = {
    [sym_verbatim] = STATE(27),
    [sym_environment] = STATE(27),
    [aux_sym__text_mode_repeat1] = STATE(27),
    [sym_text_group] = STATE(27),
    [sym__control_sequence] = STATE(27),
    [sym_open_env] = STATE(7),
    [sym_control_symbol] = STATE(27),
    [sym_control_word] = STATE(27),
    [sym_begin_command] = STATE(8),
    [sym_begin_group] = ACTIONS(5),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(11),
    [sym_text] = ACTIONS(59),
    [sym_end_group] = ACTIONS(29),
  },
  [16] = {
    [sym_verbatim] = STATE(16),
    [sym_environment] = STATE(16),
    [aux_sym__text_mode_repeat1] = STATE(16),
    [sym_text_group] = STATE(16),
    [sym__control_sequence] = STATE(16),
    [sym_open_env] = STATE(7),
    [sym_control_symbol] = STATE(16),
    [sym_control_word] = STATE(16),
    [sym_begin_command] = STATE(8),
    [sym_begin_group] = ACTIONS(61),
    [ts_builtin_sym_end] = ACTIONS(64),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(66),
    [sym_text] = ACTIONS(69),
  },
  [17] = {
    [anon_sym_end] = ACTIONS(72),
    [sym_comment] = ACTIONS(3),
    [sym_symbol] = ACTIONS(15),
    [anon_sym_begin] = ACTIONS(17),
    [anon_sym_verb] = ACTIONS(19),
    [sym_letters] = ACTIONS(21),
  },
  [18] = {
    [sym_begin_group] = ACTIONS(74),
    [ts_builtin_sym_end] = ACTIONS(74),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(74),
    [sym_text] = ACTIONS(74),
    [sym_end_group] = ACTIONS(74),
  },
  [19] = {
    [sym__escape_char] = ACTIONS(76),
    [sym_comment] = ACTIONS(9),
  },
  [20] = {
    [sym_begin_group] = ACTIONS(78),
    [sym_comment] = ACTIONS(9),
  },
  [21] = {
    [sym_verbatim] = STATE(30),
    [sym_environment] = STATE(30),
    [aux_sym__text_mode_repeat1] = STATE(30),
    [sym_text_group] = STATE(30),
    [sym__control_sequence] = STATE(30),
    [sym_open_env] = STATE(7),
    [sym_control_symbol] = STATE(30),
    [sym_control_word] = STATE(30),
    [sym_begin_command] = STATE(8),
    [sym_begin_group] = ACTIONS(5),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(11),
    [sym_text] = ACTIONS(80),
  },
  [22] = {
    [sym_close_env] = STATE(32),
    [sym_end_command] = STATE(20),
    [sym__escape_char] = ACTIONS(82),
    [sym_comment] = ACTIONS(9),
  },
  [23] = {
    [sym__control_sequence] = STATE(37),
    [sym__simple_group] = STATE(36),
    [aux_sym__simple_group_repeat1] = STATE(37),
    [sym_inline_math_shift] = STATE(37),
    [sym_control_symbol] = STATE(37),
    [sym_control_word] = STATE(37),
    [sym_display_math_shift] = ACTIONS(84),
    [sym_comment] = ACTIONS(9),
    [anon_sym_DOLLAR] = ACTIONS(86),
    [sym__escape_char] = ACTIONS(88),
    [sym_text] = ACTIONS(84),
    [sym_end_group] = ACTIONS(90),
  },
  [24] = {
    [sym_begin_group] = ACTIONS(92),
    [ts_builtin_sym_end] = ACTIONS(92),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(92),
    [sym_text] = ACTIONS(92),
    [sym_end_group] = ACTIONS(92),
  },
  [25] = {
    [sym_verb] = ACTIONS(94),
    [sym_comment] = ACTIONS(9),
  },
  [26] = {
    [sym_begin_group] = ACTIONS(96),
    [ts_builtin_sym_end] = ACTIONS(96),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(96),
    [sym_text] = ACTIONS(96),
    [sym_end_group] = ACTIONS(96),
  },
  [27] = {
    [sym_verbatim] = STATE(27),
    [sym_environment] = STATE(27),
    [aux_sym__text_mode_repeat1] = STATE(27),
    [sym_text_group] = STATE(27),
    [sym__control_sequence] = STATE(27),
    [sym_open_env] = STATE(7),
    [sym_control_symbol] = STATE(27),
    [sym_control_word] = STATE(27),
    [sym_begin_command] = STATE(8),
    [sym_begin_group] = ACTIONS(61),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(66),
    [sym_text] = ACTIONS(98),
    [sym_end_group] = ACTIONS(64),
  },
  [28] = {
    [sym_begin_group] = ACTIONS(101),
    [sym_comment] = ACTIONS(9),
  },
  [29] = {
    [sym__control_sequence] = STATE(37),
    [sym__simple_group] = STATE(40),
    [aux_sym__simple_group_repeat1] = STATE(37),
    [sym_inline_math_shift] = STATE(37),
    [sym_control_symbol] = STATE(37),
    [sym_control_word] = STATE(37),
    [sym_display_math_shift] = ACTIONS(84),
    [sym_comment] = ACTIONS(9),
    [anon_sym_DOLLAR] = ACTIONS(86),
    [sym__escape_char] = ACTIONS(88),
    [sym_text] = ACTIONS(84),
    [sym_end_group] = ACTIONS(103),
  },
  [30] = {
    [sym_verbatim] = STATE(30),
    [sym_environment] = STATE(30),
    [aux_sym__text_mode_repeat1] = STATE(30),
    [sym_text_group] = STATE(30),
    [sym__control_sequence] = STATE(30),
    [sym_open_env] = STATE(7),
    [sym_control_symbol] = STATE(30),
    [sym_control_word] = STATE(30),
    [sym_begin_command] = STATE(8),
    [sym_begin_group] = ACTIONS(61),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(66),
    [sym_text] = ACTIONS(105),
  },
  [31] = {
    [anon_sym_end] = ACTIONS(108),
    [sym_comment] = ACTIONS(9),
  },
  [32] = {
    [sym_begin_group] = ACTIONS(110),
    [ts_builtin_sym_end] = ACTIONS(110),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(110),
    [sym_text] = ACTIONS(110),
    [sym_end_group] = ACTIONS(110),
  },
  [33] = {
    [sym_display_math_shift] = ACTIONS(112),
    [sym_comment] = ACTIONS(9),
    [anon_sym_DOLLAR] = ACTIONS(114),
    [sym__escape_char] = ACTIONS(112),
    [sym_text] = ACTIONS(112),
    [sym_end_group] = ACTIONS(112),
  },
  [34] = {
    [sym_comment] = ACTIONS(3),
    [sym_symbol] = ACTIONS(15),
    [sym_letters] = ACTIONS(116),
  },
  [35] = {
    [sym_begin_group] = ACTIONS(118),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(118),
    [sym_text] = ACTIONS(118),
  },
  [36] = {
    [sym_comment] = ACTIONS(9),
    [sym_end_group] = ACTIONS(120),
  },
  [37] = {
    [sym__control_sequence] = STATE(42),
    [aux_sym__simple_group_repeat1] = STATE(42),
    [sym_inline_math_shift] = STATE(42),
    [sym_control_symbol] = STATE(42),
    [sym_control_word] = STATE(42),
    [sym_display_math_shift] = ACTIONS(122),
    [sym_comment] = ACTIONS(9),
    [anon_sym_DOLLAR] = ACTIONS(86),
    [sym__escape_char] = ACTIONS(88),
    [sym_text] = ACTIONS(122),
    [sym_end_group] = ACTIONS(124),
  },
  [38] = {
    [sym_begin_group] = ACTIONS(126),
    [ts_builtin_sym_end] = ACTIONS(126),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(126),
    [sym_text] = ACTIONS(126),
    [sym_end_group] = ACTIONS(126),
  },
  [39] = {
    [sym_begin_group] = ACTIONS(128),
    [ts_builtin_sym_end] = ACTIONS(128),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(128),
    [sym_text] = ACTIONS(128),
    [sym_end_group] = ACTIONS(128),
  },
  [40] = {
    [sym_comment] = ACTIONS(9),
    [sym_end_group] = ACTIONS(130),
  },
  [41] = {
    [sym_begin_group] = ACTIONS(132),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(132),
    [sym_text] = ACTIONS(132),
  },
  [42] = {
    [sym__control_sequence] = STATE(42),
    [aux_sym__simple_group_repeat1] = STATE(42),
    [sym_inline_math_shift] = STATE(42),
    [sym_control_symbol] = STATE(42),
    [sym_control_word] = STATE(42),
    [sym_display_math_shift] = ACTIONS(134),
    [sym_comment] = ACTIONS(9),
    [anon_sym_DOLLAR] = ACTIONS(137),
    [sym__escape_char] = ACTIONS(140),
    [sym_text] = ACTIONS(134),
    [sym_end_group] = ACTIONS(143),
  },
  [43] = {
    [sym_begin_group] = ACTIONS(145),
    [ts_builtin_sym_end] = ACTIONS(145),
    [sym_comment] = ACTIONS(9),
    [sym__escape_char] = ACTIONS(145),
    [sym_text] = ACTIONS(145),
    [sym_end_group] = ACTIONS(145),
  },
};

static TSParseActionEntry ts_parse_actions[] = {
  [0] = {.count = 0, .reusable = false},
  [1] = {.count = 1, .reusable = false}, RECOVER(),
  [3] = {.count = 1, .reusable = false}, SHIFT_EXTRA(),
  [5] = {.count = 1, .reusable = true}, SHIFT(3),
  [7] = {.count = 1, .reusable = true}, REDUCE(sym_program, 0),
  [9] = {.count = 1, .reusable = true}, SHIFT_EXTRA(),
  [11] = {.count = 1, .reusable = true}, SHIFT(2),
  [13] = {.count = 1, .reusable = true}, SHIFT(5),
  [15] = {.count = 1, .reusable = false}, SHIFT(9),
  [17] = {.count = 1, .reusable = false}, SHIFT(10),
  [19] = {.count = 1, .reusable = false}, SHIFT(11),
  [21] = {.count = 1, .reusable = false}, SHIFT(12),
  [23] = {.count = 1, .reusable = true}, SHIFT(15),
  [25] = {.count = 1, .reusable = true}, SHIFT(13),
  [27] = {.count = 1, .reusable = true}, REDUCE(sym_program, 1),
  [29] = {.count = 1, .reusable = true}, REDUCE(sym__text_mode, 1),
  [31] = {.count = 1, .reusable = true}, SHIFT(16),
  [33] = {.count = 1, .reusable = true},  ACCEPT_INPUT(),
  [35] = {.count = 1, .reusable = true}, SHIFT(17),
  [37] = {.count = 1, .reusable = true}, SHIFT(21),
  [39] = {.count = 1, .reusable = true}, SHIFT(23),
  [41] = {.count = 1, .reusable = true}, REDUCE(sym_control_symbol, 2),
  [43] = {.count = 1, .reusable = false}, REDUCE(sym_control_symbol, 2),
  [45] = {.count = 1, .reusable = true}, REDUCE(sym_begin_command, 2),
  [47] = {.count = 1, .reusable = true}, SHIFT(24),
  [49] = {.count = 1, .reusable = true}, SHIFT(25),
  [51] = {.count = 1, .reusable = true}, REDUCE(sym_control_word, 2),
  [53] = {.count = 1, .reusable = false}, REDUCE(sym_control_word, 2),
  [55] = {.count = 1, .reusable = true}, REDUCE(sym_text_group, 2),
  [57] = {.count = 1, .reusable = true}, SHIFT(26),
  [59] = {.count = 1, .reusable = true}, SHIFT(27),
  [61] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(3),
  [64] = {.count = 1, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2),
  [66] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(2),
  [69] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(16),
  [72] = {.count = 1, .reusable = false}, SHIFT(28),
  [74] = {.count = 1, .reusable = true}, REDUCE(sym_environment, 2),
  [76] = {.count = 1, .reusable = true}, REDUCE(sym_env_body, 1),
  [78] = {.count = 1, .reusable = true}, SHIFT(29),
  [80] = {.count = 1, .reusable = true}, SHIFT(30),
  [82] = {.count = 1, .reusable = true}, SHIFT(31),
  [84] = {.count = 1, .reusable = true}, SHIFT(37),
  [86] = {.count = 1, .reusable = false}, SHIFT(33),
  [88] = {.count = 1, .reusable = true}, SHIFT(34),
  [90] = {.count = 1, .reusable = true}, SHIFT(35),
  [92] = {.count = 1, .reusable = true}, REDUCE(sym_verbatim, 3),
  [94] = {.count = 1, .reusable = true}, SHIFT(38),
  [96] = {.count = 1, .reusable = true}, REDUCE(sym_text_group, 3),
  [98] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(27),
  [101] = {.count = 1, .reusable = true}, REDUCE(sym_end_command, 2),
  [103] = {.count = 1, .reusable = true}, SHIFT(39),
  [105] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(30),
  [108] = {.count = 1, .reusable = true}, SHIFT(28),
  [110] = {.count = 1, .reusable = true}, REDUCE(sym_environment, 3),
  [112] = {.count = 1, .reusable = true}, REDUCE(sym_inline_math_shift, 1),
  [114] = {.count = 1, .reusable = false}, REDUCE(sym_inline_math_shift, 1),
  [116] = {.count = 1, .reusable = true}, SHIFT(12),
  [118] = {.count = 1, .reusable = true}, REDUCE(sym_open_env, 3),
  [120] = {.count = 1, .reusable = true}, SHIFT(41),
  [122] = {.count = 1, .reusable = true}, SHIFT(42),
  [124] = {.count = 1, .reusable = true}, REDUCE(sym__simple_group, 1),
  [126] = {.count = 1, .reusable = true}, REDUCE(sym_verbatim, 4),
  [128] = {.count = 1, .reusable = true}, REDUCE(sym_close_env, 3),
  [130] = {.count = 1, .reusable = true}, SHIFT(43),
  [132] = {.count = 1, .reusable = true}, REDUCE(sym_open_env, 4),
  [134] = {.count = 2, .reusable = true}, REDUCE(aux_sym__simple_group_repeat1, 2), SHIFT_REPEAT(42),
  [137] = {.count = 2, .reusable = false}, REDUCE(aux_sym__simple_group_repeat1, 2), SHIFT_REPEAT(33),
  [140] = {.count = 2, .reusable = true}, REDUCE(aux_sym__simple_group_repeat1, 2), SHIFT_REPEAT(34),
  [143] = {.count = 1, .reusable = true}, REDUCE(aux_sym__simple_group_repeat1, 2),
  [145] = {.count = 1, .reusable = true}, REDUCE(sym_close_env, 4),
};

void *tree_sitter_latex_external_scanner_create();
void tree_sitter_latex_external_scanner_destroy(void *);
bool tree_sitter_latex_external_scanner_scan(void *, TSLexer *, const bool *);
unsigned tree_sitter_latex_external_scanner_serialize(void *, char *);
void tree_sitter_latex_external_scanner_deserialize(void *, const char *, unsigned);

#ifdef _WIN32
#define extern __declspec(dllexport)
#endif

extern const TSLanguage *tree_sitter_latex() {
  static TSLanguage language = {
    .version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .symbol_metadata = ts_symbol_metadata,
    .parse_table = (const unsigned short *)ts_parse_table,
    .parse_actions = ts_parse_actions,
    .lex_modes = ts_lex_modes,
    .symbol_names = ts_symbol_names,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .lex_fn = ts_lex,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
    .external_scanner = {
      (const bool *)ts_external_scanner_states,
      ts_external_scanner_symbol_map,
      tree_sitter_latex_external_scanner_create,
      tree_sitter_latex_external_scanner_destroy,
      tree_sitter_latex_external_scanner_scan,
      tree_sitter_latex_external_scanner_serialize,
      tree_sitter_latex_external_scanner_deserialize,
    },
  };
  return &language;
}
