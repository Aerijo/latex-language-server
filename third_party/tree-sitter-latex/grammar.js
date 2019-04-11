/**
 * This grammar targets a broader version of LaTeX than I would use
 * for syntax highlighting. Only the bare minimum for accurate highlighting
 * is present; the interpretation of a command and environment is left to
 * the server.
 *
 * Exceptions include commands like `\verb`, which would probably break the
 * grammar if they aren't special cased. I'm sure I'll find more special cases
 * as I go, but the server is predominantly intended to be used on content---
 * not fancy TeX definitions.
 */
module.exports = grammar({
  name: "latex",

  externals: $ => [
    $._error,
    $.verbatim,
    $.star,
    $.control_symbol,
    $.control_word,
    $.begin_env,
    $.end_env,
    $.inline_math_start,
    $.inline_math_end,
    $.display_math_start,
    $.display_math_end,
    $.math_shift_error,
  ],

  word: $ => $.letters,

  extras: $ => [
    $.comment
  ],

  rules: {
    program: $ => optional($._text_mode),

    /** TERMINAL SYMBOLS */

    _escape_char:    _ => '\\',
    begin_group:     _ => '{',
    end_group:       _ => '}',
    math_shift:      _ => '$',

    inline_math_shift: _ => '$',
    display_math_shift: _ => '$$',

    text: _ => /[^\$%\{\}\\]+/,

    symbol:  _ => /[^a-zA-Z@]/,
    letters: _ => /[a-zA-Z@]+/,
    _trailing_space: _ => /[\s\t\n]+/,

    comment: _ => token(seq('%', /.*\n?/)),


    /** NONTERMINAL SYMBOLS */

    _text_mode: $ => repeat1(choice(
      $._control_sequence,
      $.group,
      $.text,
      $.environment,
      $.verbatim,
      $.math_shift_error,
      $.inline_math,
      $.display_math,
    )),

    _control_sequence: $ => choice(
      $.control_symbol,
      $.control_word
    ),

    group: $ => seq($.begin_group, optional($._text_mode), $.end_group),

    inline_math: $ => seq($.inline_math_start, optional($._text_mode), $.inline_math_end),

    display_math: $ => seq($.display_math_start, optional($._text_mode), $.display_math_end),

    environment: $ => seq($.open_env, optional($.env_body), $.close_env),

    env_body: $ => $._text_mode,

    open_env: $ => seq($.begin_env, repeat(/\s+/), $.group),

    close_env: $ => seq($.end_env, repeat(/\s+/), $.group),
  }
})
