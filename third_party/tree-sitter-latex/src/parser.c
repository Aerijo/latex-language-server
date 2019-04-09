#include <tree_sitter/parser.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 9
#define STATE_COUNT 63
#define SYMBOL_COUNT 30
#define ALIAS_COUNT 0
#define TOKEN_COUNT 19
#define EXTERNAL_TOKEN_COUNT 8
#define MAX_ALIAS_SEQUENCE_LENGTH 5

enum {
  sym_letters = 1,
  sym__escape_char = 2,
  sym_begin_group = 3,
  sym_end_group = 4,
  anon_sym_DOLLAR = 5,
  sym_text = 6,
  sym_symbol = 7,
  sym__trailing_space = 8,
  sym_comment = 9,
  aux_sym_open_env_token1 = 10,
  sym__error = 11,
  sym_verb_body = 12,
  sym_star = 13,
  sym_control_symbol = 14,
  sym_control_word = 15,
  sym_begin_env = 16,
  sym_end_env = 17,
  sym_verb_command = 18,
  sym_program = 19,
  sym__text_mode = 20,
  sym_text_group = 21,
  sym__control_sequence = 22,
  sym_verbatim = 23,
  sym_environment = 24,
  sym_env_body = 25,
  sym_open_env = 26,
  sym_close_env = 27,
  aux_sym__text_mode_repeat1 = 28,
  aux_sym_open_env_repeat1 = 29,
};

static const char *ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [sym_letters] = "letters",
  [sym__escape_char] = "_escape_char",
  [sym_begin_group] = "begin_group",
  [sym_end_group] = "end_group",
  [anon_sym_DOLLAR] = "$",
  [sym_text] = "text",
  [sym_symbol] = "symbol",
  [sym__trailing_space] = "_trailing_space",
  [sym_comment] = "comment",
  [aux_sym_open_env_token1] = "open_env_token1",
  [sym__error] = "_error",
  [sym_verb_body] = "verb_body",
  [sym_star] = "star",
  [sym_control_symbol] = "control_symbol",
  [sym_control_word] = "control_word",
  [sym_begin_env] = "begin_env",
  [sym_end_env] = "end_env",
  [sym_verb_command] = "verb_command",
  [sym_program] = "program",
  [sym__text_mode] = "_text_mode",
  [sym_text_group] = "text_group",
  [sym__control_sequence] = "_control_sequence",
  [sym_verbatim] = "verbatim",
  [sym_environment] = "environment",
  [sym_env_body] = "env_body",
  [sym_open_env] = "open_env",
  [sym_close_env] = "close_env",
  [aux_sym__text_mode_repeat1] = "_text_mode_repeat1",
  [aux_sym_open_env_repeat1] = "open_env_repeat1",
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [sym_letters] = {
    .visible = true,
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
  [sym_text] = {
    .visible = true,
    .named = true,
  },
  [sym_symbol] = {
    .visible = true,
    .named = true,
  },
  [sym__trailing_space] = {
    .visible = false,
    .named = true,
  },
  [sym_comment] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_open_env_token1] = {
    .visible = false,
    .named = false,
  },
  [sym__error] = {
    .visible = false,
    .named = true,
  },
  [sym_verb_body] = {
    .visible = true,
    .named = true,
  },
  [sym_star] = {
    .visible = true,
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
  [sym_begin_env] = {
    .visible = true,
    .named = true,
  },
  [sym_end_env] = {
    .visible = true,
    .named = true,
  },
  [sym_verb_command] = {
    .visible = true,
    .named = true,
  },
  [sym_program] = {
    .visible = true,
    .named = true,
  },
  [sym__text_mode] = {
    .visible = false,
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
  [sym_close_env] = {
    .visible = true,
    .named = true,
  },
  [aux_sym__text_mode_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_open_env_repeat1] = {
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
      if (lookahead == '{')
        ADVANCE(5);
      if (lookahead == '}')
        ADVANCE(6);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        ADVANCE(7);
      if (('#' <= lookahead && lookahead <= '&') ||
          lookahead == '^' ||
          lookahead == '_' ||
          lookahead == '~')
        ADVANCE(8);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(9);
      if (lookahead != 0)
        ADVANCE(10);
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
        ADVANCE(11);
      if (lookahead != 0)
        ADVANCE(12);
      END_STATE();
    case 4:
      ACCEPT_TOKEN(sym__escape_char);
      END_STATE();
    case 5:
      ACCEPT_TOKEN(sym_begin_group);
      END_STATE();
    case 6:
      ACCEPT_TOKEN(sym_end_group);
      END_STATE();
    case 7:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        ADVANCE(13);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(14);
      END_STATE();
    case 8:
      ACCEPT_TOKEN(sym_symbol);
      END_STATE();
    case 9:
      ACCEPT_TOKEN(sym_text);
      if (('@' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(9);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(14);
      END_STATE();
    case 10:
      ACCEPT_TOKEN(sym_text);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(14);
      END_STATE();
    case 11:
      ACCEPT_TOKEN(sym_comment);
      END_STATE();
    case 12:
      ACCEPT_TOKEN(sym_comment);
      if (lookahead == '\n')
        ADVANCE(11);
      if (lookahead != 0)
        ADVANCE(12);
      END_STATE();
    case 13:
      ACCEPT_TOKEN(sym_text);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        ADVANCE(13);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(14);
      END_STATE();
    case 14:
      ACCEPT_TOKEN(sym_text);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '{' &&
          lookahead != '}' &&
          lookahead != '~')
        ADVANCE(14);
      END_STATE();
    case 15:
      if (lookahead == 0)
        ADVANCE(1);
      if (lookahead == '%')
        ADVANCE(12);
      if (lookahead == '{')
        ADVANCE(5);
      if (lookahead == '}')
        ADVANCE(6);
      if (lookahead != 0 &&
          (lookahead < '#' || '&' < lookahead) &&
          lookahead != '\\' &&
          lookahead != '^' &&
          lookahead != '_' &&
          lookahead != '~')
        ADVANCE(14);
      END_STATE();
    case 16:
      if (lookahead == '%')
        ADVANCE(12);
      if (lookahead == '{')
        ADVANCE(5);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        ADVANCE(17);
      END_STATE();
    case 17:
      ACCEPT_TOKEN(aux_sym_open_env_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        ADVANCE(17);
      END_STATE();
    default:
      return false;
  }
}

static bool ts_lex_keywords(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  switch (state) {
    case 0:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    default:
      return false;
  }
}

static TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0, .external_lex_state = 1},
  [1] = {.lex_state = 15, .external_lex_state = 2},
  [2] = {.lex_state = 15, .external_lex_state = 2},
  [3] = {.lex_state = 16},
  [4] = {.lex_state = 15, .external_lex_state = 3},
  [5] = {.lex_state = 15},
  [6] = {.lex_state = 15, .external_lex_state = 4},
  [7] = {.lex_state = 15},
  [8] = {.lex_state = 15, .external_lex_state = 2},
  [9] = {.lex_state = 15, .external_lex_state = 2},
  [10] = {.lex_state = 15},
  [11] = {.lex_state = 15, .external_lex_state = 2},
  [12] = {.lex_state = 15, .external_lex_state = 2},
  [13] = {.lex_state = 16},
  [14] = {.lex_state = 15, .external_lex_state = 2},
  [15] = {.lex_state = 15, .external_lex_state = 5},
  [16] = {.lex_state = 15, .external_lex_state = 2},
  [17] = {.lex_state = 15, .external_lex_state = 3},
  [18] = {.lex_state = 16},
  [19] = {.lex_state = 15, .external_lex_state = 6},
  [20] = {.lex_state = 15, .external_lex_state = 6},
  [21] = {.lex_state = 15, .external_lex_state = 4},
  [22] = {.lex_state = 15, .external_lex_state = 2},
  [23] = {.lex_state = 15, .external_lex_state = 4},
  [24] = {.lex_state = 15, .external_lex_state = 2},
  [25] = {.lex_state = 15, .external_lex_state = 2},
  [26] = {.lex_state = 15, .external_lex_state = 2},
  [27] = {.lex_state = 15, .external_lex_state = 4},
  [28] = {.lex_state = 15},
  [29] = {.lex_state = 15, .external_lex_state = 2},
  [30] = {.lex_state = 16},
  [31] = {.lex_state = 15, .external_lex_state = 2},
  [32] = {.lex_state = 15, .external_lex_state = 4},
  [33] = {.lex_state = 15},
  [34] = {.lex_state = 15, .external_lex_state = 4},
  [35] = {.lex_state = 15, .external_lex_state = 5},
  [36] = {.lex_state = 15, .external_lex_state = 2},
  [37] = {.lex_state = 16},
  [38] = {.lex_state = 15, .external_lex_state = 2},
  [39] = {.lex_state = 16},
  [40] = {.lex_state = 15, .external_lex_state = 6},
  [41] = {.lex_state = 15, .external_lex_state = 4},
  [42] = {.lex_state = 15, .external_lex_state = 4},
  [43] = {.lex_state = 15, .external_lex_state = 4},
  [44] = {.lex_state = 15},
  [45] = {.lex_state = 15, .external_lex_state = 4},
  [46] = {.lex_state = 15, .external_lex_state = 4},
  [47] = {.lex_state = 15, .external_lex_state = 2},
  [48] = {.lex_state = 15},
  [49] = {.lex_state = 15, .external_lex_state = 2},
  [50] = {.lex_state = 15, .external_lex_state = 2},
  [51] = {.lex_state = 16},
  [52] = {.lex_state = 15, .external_lex_state = 4},
  [53] = {.lex_state = 15, .external_lex_state = 4},
  [54] = {.lex_state = 15, .external_lex_state = 2},
  [55] = {.lex_state = 15},
  [56] = {.lex_state = 15, .external_lex_state = 4},
  [57] = {.lex_state = 15},
  [58] = {.lex_state = 15, .external_lex_state = 2},
  [59] = {.lex_state = 15, .external_lex_state = 2},
  [60] = {.lex_state = 15, .external_lex_state = 4},
  [61] = {.lex_state = 15},
  [62] = {.lex_state = 15, .external_lex_state = 4},
};

enum {
  ts_external_token__error = 0,
  ts_external_token_verb_body = 1,
  ts_external_token_star = 2,
  ts_external_token_control_symbol = 3,
  ts_external_token_control_word = 4,
  ts_external_token_begin_env = 5,
  ts_external_token_end_env = 6,
  ts_external_token_verb_command = 7,
};

static TSSymbol ts_external_scanner_symbol_map[EXTERNAL_TOKEN_COUNT] = {
  [ts_external_token__error] = sym__error,
  [ts_external_token_verb_body] = sym_verb_body,
  [ts_external_token_star] = sym_star,
  [ts_external_token_control_symbol] = sym_control_symbol,
  [ts_external_token_control_word] = sym_control_word,
  [ts_external_token_begin_env] = sym_begin_env,
  [ts_external_token_end_env] = sym_end_env,
  [ts_external_token_verb_command] = sym_verb_command,
};

static bool ts_external_scanner_states[7][EXTERNAL_TOKEN_COUNT] = {
  [1] = {
    [ts_external_token__error] = true,
    [ts_external_token_verb_command] = true,
    [ts_external_token_control_word] = true,
    [ts_external_token_verb_body] = true,
    [ts_external_token_begin_env] = true,
    [ts_external_token_star] = true,
    [ts_external_token_end_env] = true,
    [ts_external_token_control_symbol] = true,
  },
  [2] = {
    [ts_external_token_begin_env] = true,
    [ts_external_token_verb_command] = true,
    [ts_external_token_control_word] = true,
    [ts_external_token_control_symbol] = true,
  },
  [3] = {
    [ts_external_token_verb_body] = true,
    [ts_external_token_star] = true,
  },
  [4] = {
    [ts_external_token_begin_env] = true,
    [ts_external_token_verb_command] = true,
    [ts_external_token_control_word] = true,
    [ts_external_token_end_env] = true,
    [ts_external_token_control_symbol] = true,
  },
  [5] = {
    [ts_external_token_verb_body] = true,
  },
  [6] = {
    [ts_external_token_end_env] = true,
  },
};

static uint16_t ts_parse_table[STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [sym__error] = ACTIONS(1),
    [sym_comment] = ACTIONS(3),
    [anon_sym_DOLLAR] = ACTIONS(1),
    [sym__escape_char] = ACTIONS(1),
    [sym_text] = ACTIONS(1),
    [sym_begin_env] = ACTIONS(1),
    [sym_star] = ACTIONS(1),
    [sym__trailing_space] = ACTIONS(1),
    [sym_end_group] = ACTIONS(1),
    [sym_letters] = ACTIONS(1),
    [ts_builtin_sym_end] = ACTIONS(1),
    [sym_verb_command] = ACTIONS(1),
    [sym_control_word] = ACTIONS(1),
    [sym_verb_body] = ACTIONS(1),
    [aux_sym_open_env_token1] = ACTIONS(1),
    [sym_begin_group] = ACTIONS(1),
    [sym_symbol] = ACTIONS(1),
    [sym_end_env] = ACTIONS(1),
    [sym_control_symbol] = ACTIONS(1),
  },
  [1] = {
    [sym__text_mode] = STATE(5),
    [sym_open_env] = STATE(6),
    [sym_program] = STATE(7),
    [sym_text_group] = STATE(8),
    [sym__control_sequence] = STATE(8),
    [aux_sym__text_mode_repeat1] = STATE(8),
    [sym_verbatim] = STATE(8),
    [sym_environment] = STATE(8),
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(7),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(11),
    [sym_text] = ACTIONS(11),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_control_symbol] = ACTIONS(11),
  },
  [2] = {
    [sym__control_sequence] = STATE(11),
    [sym__text_mode] = STATE(10),
    [aux_sym__text_mode_repeat1] = STATE(11),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(11),
    [sym_environment] = STATE(11),
    [sym_text_group] = STATE(11),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(17),
    [sym_text] = ACTIONS(17),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_group] = ACTIONS(19),
    [sym_control_symbol] = ACTIONS(17),
  },
  [3] = {
    [aux_sym_open_env_repeat1] = STATE(13),
    [sym_begin_group] = ACTIONS(21),
    [aux_sym_open_env_token1] = ACTIONS(23),
    [sym_comment] = ACTIONS(5),
  },
  [4] = {
    [sym_verb_body] = ACTIONS(25),
    [sym_star] = ACTIONS(27),
    [sym_comment] = ACTIONS(5),
  },
  [5] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(29),
  },
  [6] = {
    [sym__text_mode] = STATE(19),
    [sym_env_body] = STATE(20),
    [sym_open_env] = STATE(21),
    [sym_text_group] = STATE(23),
    [sym__control_sequence] = STATE(23),
    [sym_close_env] = STATE(22),
    [aux_sym__text_mode_repeat1] = STATE(23),
    [sym_verbatim] = STATE(23),
    [sym_environment] = STATE(23),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(31),
    [sym_control_word] = ACTIONS(33),
    [sym_text] = ACTIONS(33),
    [sym_begin_group] = ACTIONS(35),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_env] = ACTIONS(37),
    [sym_control_symbol] = ACTIONS(33),
  },
  [7] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(39),
  },
  [8] = {
    [sym__control_sequence] = STATE(24),
    [aux_sym__text_mode_repeat1] = STATE(24),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(24),
    [sym_environment] = STATE(24),
    [sym_text_group] = STATE(24),
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(41),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(43),
    [sym_text] = ACTIONS(43),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_control_symbol] = ACTIONS(43),
  },
  [9] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(45),
    [sym_verb_command] = ACTIONS(45),
    [sym_control_word] = ACTIONS(45),
    [sym_text] = ACTIONS(45),
    [sym_begin_group] = ACTIONS(45),
    [sym_begin_env] = ACTIONS(45),
    [sym_end_group] = ACTIONS(45),
    [sym_control_symbol] = ACTIONS(45),
  },
  [10] = {
    [sym_comment] = ACTIONS(5),
    [sym_end_group] = ACTIONS(47),
  },
  [11] = {
    [sym__control_sequence] = STATE(26),
    [aux_sym__text_mode_repeat1] = STATE(26),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(26),
    [sym_environment] = STATE(26),
    [sym_text_group] = STATE(26),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(49),
    [sym_text] = ACTIONS(49),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_group] = ACTIONS(41),
    [sym_control_symbol] = ACTIONS(49),
  },
  [12] = {
    [sym__control_sequence] = STATE(11),
    [sym__text_mode] = STATE(28),
    [aux_sym__text_mode_repeat1] = STATE(11),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(11),
    [sym_environment] = STATE(11),
    [sym_text_group] = STATE(11),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(17),
    [sym_text] = ACTIONS(17),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_group] = ACTIONS(51),
    [sym_control_symbol] = ACTIONS(17),
  },
  [13] = {
    [aux_sym_open_env_repeat1] = STATE(30),
    [sym_begin_group] = ACTIONS(53),
    [aux_sym_open_env_token1] = ACTIONS(55),
    [sym_comment] = ACTIONS(5),
  },
  [14] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(57),
    [sym_verb_command] = ACTIONS(57),
    [sym_control_word] = ACTIONS(57),
    [sym_text] = ACTIONS(57),
    [sym_begin_group] = ACTIONS(57),
    [sym_begin_env] = ACTIONS(57),
    [sym_end_group] = ACTIONS(57),
    [sym_control_symbol] = ACTIONS(57),
  },
  [15] = {
    [sym_verb_body] = ACTIONS(59),
    [sym_comment] = ACTIONS(5),
  },
  [16] = {
    [sym__control_sequence] = STATE(11),
    [sym__text_mode] = STATE(33),
    [aux_sym__text_mode_repeat1] = STATE(11),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(11),
    [sym_environment] = STATE(11),
    [sym_text_group] = STATE(11),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(17),
    [sym_text] = ACTIONS(17),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_group] = ACTIONS(61),
    [sym_control_symbol] = ACTIONS(17),
  },
  [17] = {
    [sym_verb_body] = ACTIONS(63),
    [sym_star] = ACTIONS(65),
    [sym_comment] = ACTIONS(5),
  },
  [18] = {
    [aux_sym_open_env_repeat1] = STATE(37),
    [sym_begin_group] = ACTIONS(67),
    [aux_sym_open_env_token1] = ACTIONS(69),
    [sym_comment] = ACTIONS(5),
  },
  [19] = {
    [sym_comment] = ACTIONS(5),
    [sym_end_env] = ACTIONS(71),
  },
  [20] = {
    [sym_close_env] = STATE(38),
    [sym_comment] = ACTIONS(5),
    [sym_end_env] = ACTIONS(37),
  },
  [21] = {
    [sym__text_mode] = STATE(19),
    [sym_env_body] = STATE(40),
    [sym_open_env] = STATE(21),
    [sym_text_group] = STATE(23),
    [sym__control_sequence] = STATE(23),
    [sym_close_env] = STATE(41),
    [aux_sym__text_mode_repeat1] = STATE(23),
    [sym_verbatim] = STATE(23),
    [sym_environment] = STATE(23),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(31),
    [sym_control_word] = ACTIONS(33),
    [sym_text] = ACTIONS(33),
    [sym_begin_group] = ACTIONS(35),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_env] = ACTIONS(73),
    [sym_control_symbol] = ACTIONS(33),
  },
  [22] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(75),
    [sym_verb_command] = ACTIONS(75),
    [sym_control_word] = ACTIONS(75),
    [sym_text] = ACTIONS(75),
    [sym_begin_group] = ACTIONS(75),
    [sym_begin_env] = ACTIONS(75),
    [sym_end_group] = ACTIONS(75),
    [sym_control_symbol] = ACTIONS(75),
  },
  [23] = {
    [sym__control_sequence] = STATE(42),
    [aux_sym__text_mode_repeat1] = STATE(42),
    [sym_open_env] = STATE(21),
    [sym_verbatim] = STATE(42),
    [sym_environment] = STATE(42),
    [sym_text_group] = STATE(42),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(31),
    [sym_control_word] = ACTIONS(77),
    [sym_text] = ACTIONS(77),
    [sym_begin_group] = ACTIONS(35),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_env] = ACTIONS(41),
    [sym_control_symbol] = ACTIONS(77),
  },
  [24] = {
    [sym__control_sequence] = STATE(24),
    [aux_sym__text_mode_repeat1] = STATE(24),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(24),
    [sym_environment] = STATE(24),
    [sym_text_group] = STATE(24),
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(79),
    [sym_verb_command] = ACTIONS(81),
    [sym_control_word] = ACTIONS(84),
    [sym_text] = ACTIONS(84),
    [sym_begin_group] = ACTIONS(87),
    [sym_begin_env] = ACTIONS(90),
    [sym_control_symbol] = ACTIONS(84),
  },
  [25] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(93),
    [sym_verb_command] = ACTIONS(93),
    [sym_control_word] = ACTIONS(93),
    [sym_text] = ACTIONS(93),
    [sym_begin_group] = ACTIONS(93),
    [sym_begin_env] = ACTIONS(93),
    [sym_end_group] = ACTIONS(93),
    [sym_control_symbol] = ACTIONS(93),
  },
  [26] = {
    [sym__control_sequence] = STATE(26),
    [aux_sym__text_mode_repeat1] = STATE(26),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(26),
    [sym_environment] = STATE(26),
    [sym_text_group] = STATE(26),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(81),
    [sym_control_word] = ACTIONS(95),
    [sym_text] = ACTIONS(95),
    [sym_begin_group] = ACTIONS(87),
    [sym_begin_env] = ACTIONS(90),
    [sym_end_group] = ACTIONS(79),
    [sym_control_symbol] = ACTIONS(95),
  },
  [27] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(98),
    [sym_control_word] = ACTIONS(98),
    [sym_text] = ACTIONS(98),
    [sym_begin_group] = ACTIONS(98),
    [sym_begin_env] = ACTIONS(98),
    [sym_end_env] = ACTIONS(98),
    [sym_control_symbol] = ACTIONS(98),
  },
  [28] = {
    [sym_comment] = ACTIONS(5),
    [sym_end_group] = ACTIONS(100),
  },
  [29] = {
    [sym__control_sequence] = STATE(11),
    [sym__text_mode] = STATE(44),
    [aux_sym__text_mode_repeat1] = STATE(11),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(11),
    [sym_environment] = STATE(11),
    [sym_text_group] = STATE(11),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(17),
    [sym_text] = ACTIONS(17),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_group] = ACTIONS(100),
    [sym_control_symbol] = ACTIONS(17),
  },
  [30] = {
    [aux_sym_open_env_repeat1] = STATE(30),
    [sym_begin_group] = ACTIONS(102),
    [aux_sym_open_env_token1] = ACTIONS(104),
    [sym_comment] = ACTIONS(5),
  },
  [31] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(107),
    [sym_verb_command] = ACTIONS(107),
    [sym_control_word] = ACTIONS(107),
    [sym_text] = ACTIONS(107),
    [sym_begin_group] = ACTIONS(107),
    [sym_begin_env] = ACTIONS(107),
    [sym_end_group] = ACTIONS(107),
    [sym_control_symbol] = ACTIONS(107),
  },
  [32] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(45),
    [sym_control_word] = ACTIONS(45),
    [sym_text] = ACTIONS(45),
    [sym_begin_group] = ACTIONS(45),
    [sym_begin_env] = ACTIONS(45),
    [sym_end_env] = ACTIONS(45),
    [sym_control_symbol] = ACTIONS(45),
  },
  [33] = {
    [sym_comment] = ACTIONS(5),
    [sym_end_group] = ACTIONS(109),
  },
  [34] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(57),
    [sym_control_word] = ACTIONS(57),
    [sym_text] = ACTIONS(57),
    [sym_begin_group] = ACTIONS(57),
    [sym_begin_env] = ACTIONS(57),
    [sym_end_env] = ACTIONS(57),
    [sym_control_symbol] = ACTIONS(57),
  },
  [35] = {
    [sym_verb_body] = ACTIONS(111),
    [sym_comment] = ACTIONS(5),
  },
  [36] = {
    [sym__control_sequence] = STATE(11),
    [sym__text_mode] = STATE(48),
    [aux_sym__text_mode_repeat1] = STATE(11),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(11),
    [sym_environment] = STATE(11),
    [sym_text_group] = STATE(11),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(17),
    [sym_text] = ACTIONS(17),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_group] = ACTIONS(113),
    [sym_control_symbol] = ACTIONS(17),
  },
  [37] = {
    [aux_sym_open_env_repeat1] = STATE(30),
    [sym_begin_group] = ACTIONS(115),
    [aux_sym_open_env_token1] = ACTIONS(55),
    [sym_comment] = ACTIONS(5),
  },
  [38] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(117),
    [sym_verb_command] = ACTIONS(117),
    [sym_control_word] = ACTIONS(117),
    [sym_text] = ACTIONS(117),
    [sym_begin_group] = ACTIONS(117),
    [sym_begin_env] = ACTIONS(117),
    [sym_end_group] = ACTIONS(117),
    [sym_control_symbol] = ACTIONS(117),
  },
  [39] = {
    [aux_sym_open_env_repeat1] = STATE(51),
    [sym_begin_group] = ACTIONS(119),
    [aux_sym_open_env_token1] = ACTIONS(121),
    [sym_comment] = ACTIONS(5),
  },
  [40] = {
    [sym_close_env] = STATE(52),
    [sym_comment] = ACTIONS(5),
    [sym_end_env] = ACTIONS(73),
  },
  [41] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(75),
    [sym_control_word] = ACTIONS(75),
    [sym_text] = ACTIONS(75),
    [sym_begin_group] = ACTIONS(75),
    [sym_begin_env] = ACTIONS(75),
    [sym_end_env] = ACTIONS(75),
    [sym_control_symbol] = ACTIONS(75),
  },
  [42] = {
    [sym__control_sequence] = STATE(42),
    [aux_sym__text_mode_repeat1] = STATE(42),
    [sym_open_env] = STATE(21),
    [sym_verbatim] = STATE(42),
    [sym_environment] = STATE(42),
    [sym_text_group] = STATE(42),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(123),
    [sym_control_word] = ACTIONS(126),
    [sym_text] = ACTIONS(126),
    [sym_begin_group] = ACTIONS(129),
    [sym_begin_env] = ACTIONS(90),
    [sym_end_env] = ACTIONS(79),
    [sym_control_symbol] = ACTIONS(126),
  },
  [43] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(132),
    [sym_control_word] = ACTIONS(132),
    [sym_text] = ACTIONS(132),
    [sym_begin_group] = ACTIONS(132),
    [sym_begin_env] = ACTIONS(132),
    [sym_end_env] = ACTIONS(132),
    [sym_control_symbol] = ACTIONS(132),
  },
  [44] = {
    [sym_comment] = ACTIONS(5),
    [sym_end_group] = ACTIONS(134),
  },
  [45] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(93),
    [sym_control_word] = ACTIONS(93),
    [sym_text] = ACTIONS(93),
    [sym_begin_group] = ACTIONS(93),
    [sym_begin_env] = ACTIONS(93),
    [sym_end_env] = ACTIONS(93),
    [sym_control_symbol] = ACTIONS(93),
  },
  [46] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(107),
    [sym_control_word] = ACTIONS(107),
    [sym_text] = ACTIONS(107),
    [sym_begin_group] = ACTIONS(107),
    [sym_begin_env] = ACTIONS(107),
    [sym_end_env] = ACTIONS(107),
    [sym_control_symbol] = ACTIONS(107),
  },
  [47] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(136),
    [sym_verb_command] = ACTIONS(136),
    [sym_control_word] = ACTIONS(136),
    [sym_text] = ACTIONS(136),
    [sym_begin_group] = ACTIONS(136),
    [sym_begin_env] = ACTIONS(136),
    [sym_end_group] = ACTIONS(136),
    [sym_control_symbol] = ACTIONS(136),
  },
  [48] = {
    [sym_comment] = ACTIONS(5),
    [sym_end_group] = ACTIONS(138),
  },
  [49] = {
    [sym__control_sequence] = STATE(11),
    [sym__text_mode] = STATE(55),
    [aux_sym__text_mode_repeat1] = STATE(11),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(11),
    [sym_environment] = STATE(11),
    [sym_text_group] = STATE(11),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(17),
    [sym_text] = ACTIONS(17),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_group] = ACTIONS(138),
    [sym_control_symbol] = ACTIONS(17),
  },
  [50] = {
    [sym__control_sequence] = STATE(11),
    [sym__text_mode] = STATE(57),
    [aux_sym__text_mode_repeat1] = STATE(11),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(11),
    [sym_environment] = STATE(11),
    [sym_text_group] = STATE(11),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(17),
    [sym_text] = ACTIONS(17),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_group] = ACTIONS(140),
    [sym_control_symbol] = ACTIONS(17),
  },
  [51] = {
    [aux_sym_open_env_repeat1] = STATE(30),
    [sym_begin_group] = ACTIONS(142),
    [aux_sym_open_env_token1] = ACTIONS(55),
    [sym_comment] = ACTIONS(5),
  },
  [52] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(117),
    [sym_control_word] = ACTIONS(117),
    [sym_text] = ACTIONS(117),
    [sym_begin_group] = ACTIONS(117),
    [sym_begin_env] = ACTIONS(117),
    [sym_end_env] = ACTIONS(117),
    [sym_control_symbol] = ACTIONS(117),
  },
  [53] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(144),
    [sym_control_word] = ACTIONS(144),
    [sym_text] = ACTIONS(144),
    [sym_begin_group] = ACTIONS(144),
    [sym_begin_env] = ACTIONS(144),
    [sym_end_env] = ACTIONS(144),
    [sym_control_symbol] = ACTIONS(144),
  },
  [54] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(146),
    [sym_verb_command] = ACTIONS(146),
    [sym_control_word] = ACTIONS(146),
    [sym_text] = ACTIONS(146),
    [sym_begin_group] = ACTIONS(146),
    [sym_begin_env] = ACTIONS(146),
    [sym_end_group] = ACTIONS(146),
    [sym_control_symbol] = ACTIONS(146),
  },
  [55] = {
    [sym_comment] = ACTIONS(5),
    [sym_end_group] = ACTIONS(148),
  },
  [56] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(136),
    [sym_control_word] = ACTIONS(136),
    [sym_text] = ACTIONS(136),
    [sym_begin_group] = ACTIONS(136),
    [sym_begin_env] = ACTIONS(136),
    [sym_end_env] = ACTIONS(136),
    [sym_control_symbol] = ACTIONS(136),
  },
  [57] = {
    [sym_comment] = ACTIONS(5),
    [sym_end_group] = ACTIONS(150),
  },
  [58] = {
    [sym__control_sequence] = STATE(11),
    [sym__text_mode] = STATE(61),
    [aux_sym__text_mode_repeat1] = STATE(11),
    [sym_open_env] = STATE(6),
    [sym_verbatim] = STATE(11),
    [sym_environment] = STATE(11),
    [sym_text_group] = STATE(11),
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(9),
    [sym_control_word] = ACTIONS(17),
    [sym_text] = ACTIONS(17),
    [sym_begin_group] = ACTIONS(13),
    [sym_begin_env] = ACTIONS(15),
    [sym_end_group] = ACTIONS(150),
    [sym_control_symbol] = ACTIONS(17),
  },
  [59] = {
    [sym_comment] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(152),
    [sym_verb_command] = ACTIONS(152),
    [sym_control_word] = ACTIONS(152),
    [sym_text] = ACTIONS(152),
    [sym_begin_group] = ACTIONS(152),
    [sym_begin_env] = ACTIONS(152),
    [sym_end_group] = ACTIONS(152),
    [sym_control_symbol] = ACTIONS(152),
  },
  [60] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(146),
    [sym_control_word] = ACTIONS(146),
    [sym_text] = ACTIONS(146),
    [sym_begin_group] = ACTIONS(146),
    [sym_begin_env] = ACTIONS(146),
    [sym_end_env] = ACTIONS(146),
    [sym_control_symbol] = ACTIONS(146),
  },
  [61] = {
    [sym_comment] = ACTIONS(5),
    [sym_end_group] = ACTIONS(154),
  },
  [62] = {
    [sym_comment] = ACTIONS(5),
    [sym_verb_command] = ACTIONS(152),
    [sym_control_word] = ACTIONS(152),
    [sym_text] = ACTIONS(152),
    [sym_begin_group] = ACTIONS(152),
    [sym_begin_env] = ACTIONS(152),
    [sym_end_env] = ACTIONS(152),
    [sym_control_symbol] = ACTIONS(152),
  },
};

static TSParseActionEntry ts_parse_actions[] = {
  [0] = {.count = 0, .reusable = false},
  [1] = {.count = 1, .reusable = false}, RECOVER(),
  [3] = {.count = 1, .reusable = false}, SHIFT_EXTRA(),
  [5] = {.count = 1, .reusable = true}, SHIFT_EXTRA(),
  [7] = {.count = 1, .reusable = true}, REDUCE(sym_program, 0),
  [9] = {.count = 1, .reusable = true}, SHIFT(4),
  [11] = {.count = 1, .reusable = true}, SHIFT(8),
  [13] = {.count = 1, .reusable = true}, SHIFT(2),
  [15] = {.count = 1, .reusable = true}, SHIFT(3),
  [17] = {.count = 1, .reusable = true}, SHIFT(11),
  [19] = {.count = 1, .reusable = true}, SHIFT(9),
  [21] = {.count = 1, .reusable = true}, SHIFT(12),
  [23] = {.count = 1, .reusable = true}, SHIFT(13),
  [25] = {.count = 1, .reusable = true}, SHIFT(14),
  [27] = {.count = 1, .reusable = true}, SHIFT(15),
  [29] = {.count = 1, .reusable = true}, REDUCE(sym_program, 1),
  [31] = {.count = 1, .reusable = true}, SHIFT(17),
  [33] = {.count = 1, .reusable = true}, SHIFT(23),
  [35] = {.count = 1, .reusable = true}, SHIFT(16),
  [37] = {.count = 1, .reusable = true}, SHIFT(18),
  [39] = {.count = 1, .reusable = true},  ACCEPT_INPUT(),
  [41] = {.count = 1, .reusable = true}, REDUCE(sym__text_mode, 1),
  [43] = {.count = 1, .reusable = true}, SHIFT(24),
  [45] = {.count = 1, .reusable = true}, REDUCE(sym_text_group, 2),
  [47] = {.count = 1, .reusable = true}, SHIFT(25),
  [49] = {.count = 1, .reusable = true}, SHIFT(26),
  [51] = {.count = 1, .reusable = true}, SHIFT(27),
  [53] = {.count = 1, .reusable = true}, SHIFT(29),
  [55] = {.count = 1, .reusable = true}, SHIFT(30),
  [57] = {.count = 1, .reusable = true}, REDUCE(sym_verbatim, 2),
  [59] = {.count = 1, .reusable = true}, SHIFT(31),
  [61] = {.count = 1, .reusable = true}, SHIFT(32),
  [63] = {.count = 1, .reusable = true}, SHIFT(34),
  [65] = {.count = 1, .reusable = true}, SHIFT(35),
  [67] = {.count = 1, .reusable = true}, SHIFT(36),
  [69] = {.count = 1, .reusable = true}, SHIFT(37),
  [71] = {.count = 1, .reusable = true}, REDUCE(sym_env_body, 1),
  [73] = {.count = 1, .reusable = true}, SHIFT(39),
  [75] = {.count = 1, .reusable = true}, REDUCE(sym_environment, 2),
  [77] = {.count = 1, .reusable = true}, SHIFT(42),
  [79] = {.count = 1, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2),
  [81] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(4),
  [84] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(24),
  [87] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(2),
  [90] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(3),
  [93] = {.count = 1, .reusable = true}, REDUCE(sym_text_group, 3),
  [95] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(26),
  [98] = {.count = 1, .reusable = true}, REDUCE(sym_open_env, 3),
  [100] = {.count = 1, .reusable = true}, SHIFT(43),
  [102] = {.count = 1, .reusable = true}, REDUCE(aux_sym_open_env_repeat1, 2),
  [104] = {.count = 2, .reusable = true}, REDUCE(aux_sym_open_env_repeat1, 2), SHIFT_REPEAT(30),
  [107] = {.count = 1, .reusable = true}, REDUCE(sym_verbatim, 3),
  [109] = {.count = 1, .reusable = true}, SHIFT(45),
  [111] = {.count = 1, .reusable = true}, SHIFT(46),
  [113] = {.count = 1, .reusable = true}, SHIFT(47),
  [115] = {.count = 1, .reusable = true}, SHIFT(49),
  [117] = {.count = 1, .reusable = true}, REDUCE(sym_environment, 3),
  [119] = {.count = 1, .reusable = true}, SHIFT(50),
  [121] = {.count = 1, .reusable = true}, SHIFT(51),
  [123] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(17),
  [126] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(42),
  [129] = {.count = 2, .reusable = true}, REDUCE(aux_sym__text_mode_repeat1, 2), SHIFT_REPEAT(16),
  [132] = {.count = 1, .reusable = true}, REDUCE(sym_open_env, 4),
  [134] = {.count = 1, .reusable = true}, SHIFT(53),
  [136] = {.count = 1, .reusable = true}, REDUCE(sym_close_env, 3),
  [138] = {.count = 1, .reusable = true}, SHIFT(54),
  [140] = {.count = 1, .reusable = true}, SHIFT(56),
  [142] = {.count = 1, .reusable = true}, SHIFT(58),
  [144] = {.count = 1, .reusable = true}, REDUCE(sym_open_env, 5),
  [146] = {.count = 1, .reusable = true}, REDUCE(sym_close_env, 4),
  [148] = {.count = 1, .reusable = true}, SHIFT(59),
  [150] = {.count = 1, .reusable = true}, SHIFT(60),
  [152] = {.count = 1, .reusable = true}, REDUCE(sym_close_env, 5),
  [154] = {.count = 1, .reusable = true}, SHIFT(62),
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
    .keyword_lex_fn = ts_lex_keywords,
    .keyword_capture_token = sym_letters,
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
