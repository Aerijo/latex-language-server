module.exports = {
  text: {
    commands: {
      lstinline: {
        cs: $ => $.cs_lstinline,
        parameters: $ => [
          choice(
            seq(
              $.brack_group,
              $.verb_delim
            ),
            alias($.verb_delim_no_lbrack, $.verb_delim)
          ),
          optional(alias($.verb_body, $.verbatim)),
          choice(
            alias($.verb_end_delim, $.verb_delim),
            $.exit
          )
        ]
      }
    },
    environments: {
      lstlisting: {
        name: $ => $.env_name_lstlisting,
        beginParameters: $ => [
          optional($.brack_group),
          $.eol
        ],
        contents: $ => [
          $.verbatim
        ],
        bare: true
      }
    }
  }
}
