/**
 * This grammar targets a broader version of LaTeX than I would use
 * for syntax highlighting. Only the bare minimum for accurate highlighting
 * is prsesent; the interpretation of a command and environment is left to
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
    $.verb_body,
    $.star,
    $.control_symbol,
    $.control_word,
    $.begin_env,
    $.end_env,
    $.verb_command,
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

    text: _ => /[^\#\$%\^&_\{\}~\\][^\#\$%\^\&_\{\}~\\]*/,

    symbol:  _ => /[^a-zA-Z@]/,
    letters: _ => /[a-zA-Z@]+/,
    _trailing_space: _ => /[\s\t\n]+/,

    comment: _ => token(seq('%', /.*\n?/)),


    /** NONTERMINAL SYMBOLS */

    _text_mode: $ => prec.left(repeat1(choice(
      $._control_sequence,
      $.text_group,
      $.text,
      $.environment,
      $.verbatim,
    ))),

    _simple_group: $ => repeat1(choice(
      $._control_sequence,
      $.inline_math_shift,
      $.display_math_shift,
      $.text
    )),

    text_group: $ => seq($.begin_group, optional($._text_mode), $.end_group),

    _control_sequence: $ => choice(
      $.control_symbol,
      $.control_word
    ),

    verbatim: $ => seq($.verb_command, optional($.star), $.verb_body),

    environment: $ => seq($.open_env, optional($.env_body), $.close_env),

    env_body: $ => $._text_mode,

    open_env: $ => seq($.begin_env, repeat(/\s+/), $.begin_group, optional($._text_mode), $.end_group),

    close_env: $ => seq($.end_env, repeat(/\s+/), $.begin_group, optional($._text_mode), $.end_group),
  }
})
