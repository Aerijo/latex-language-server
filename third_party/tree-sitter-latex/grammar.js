const cmd = ($, name, {ws=true, star=false, argument=null}) => {
  const arg = [$._escape_char, alias(name, $.cs)]

  if (!star && !argument) return seq(...arg)

  if (ws && star) {
    arg.push(optional($.s_whitespace), repeat($.m_whitespace), optional(seq($.star, optional($.s_whitespace), repeat($.m_whitespace))))
    if (argument) {
      arg.push(argument)
    }
  } else if (ws && !star && argument) {
    arg.push(optional($.s_whitespace), repeat($.m_whitespace), argument)
  } else if (!ws && star) {
    arg.push(optional($.star))
  }

  let result = seq(...arg)
  if (argument) {
    result = prec.right(result)
  }

  return result

  /*
  Whitespace rules:
    1) Every line starts as (N)
    2) The state is (S) after a control word
    3) The state is (S) after a control space '\ '
    4) The state is (M) after any other control symbol
    5) If a newline is seen:
      (N) - '\par' is inserted
      (S) - Ignored
      (M) - ' ' is inserted
    6) Comments consume the newline, so the state simply switches to (N) regardless

  Extra rules:
    1) If a command can be starred, it is in state (S) after the star


  E.g.,
  ```
  (N)   \begin    (S)    \n
  (N)     % (Note newline is removed by comment)
  (N) {env} % <- this is the argument to \begin


  (N)   \section  (S)    *  (M)   \n (in M, so space)
  (N)      {name}
  ```
  */
}

const group = ($, contents, opt=true) => {
  if (contents instanceof Array) {
    contents = seq(...contents)
  }

  if (opt) {
    contents = optional(contents)
  }

  return seq($.begin_group, contents, $.end_group)
}

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
    $.verb,
    $.star, // defined in this file
    $.s_whitespace,
    $.m_whitespace,
  ],

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

    _text_mode: $ => prec.right(repeat1(choice(
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

    control_symbol: $ => cmd($, $.symbol, {ws: false}), // trailing space is not removed in a control symbol.
    control_word:   $ => cmd($, $.letters, {ws: true}),

    verbatim: $ => seq(cmd($, "verb", {ws: false, star: true}), $.verb),

    environment: $ => seq($.open_env, optional($.env_body), $.close_env),

    env_body: $ => $._text_mode,

    open_env: $ => seq(
      $.begin_command,
      group($, optional(alias($._simple_group, $.env_name))),
    ),

    begin_command: $ => cmd($, "begin", {}),

    close_env: $ => seq(
      $.end_command,
      group($, optional(alias($._simple_group, $.env_name))),
    ),

    end_command: $ => cmd($, "end", {}),
  }
})
