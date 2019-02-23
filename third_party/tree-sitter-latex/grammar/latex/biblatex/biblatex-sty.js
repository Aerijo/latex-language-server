module.exports = {
  text: {
    commands: {
      cite: {
        cs: $ => $.cs_cite,
        parameters: $ => [
          optional($.star),
          optional($.brack_group),
          optional($.brack_group),
          $._text_token
        ]
      },
      cites: {
        cs: $ => $.cs_cites,
        parameters: $ => [
          prec.right(-1,
            repeat1(
              seq(
                optional($.brack_group),
                optional($.brack_group),
                $._text_token
              )
            )
          )
        ]
      },
      nocite: {
        cs: $ => $.cs_nocite,
        parameters: $ => [$._text_token]
      },
      regexp: {
        cs: $ => $.cs_regexp,
        parameters: $ => [$._text_token]
      },
      volcite: {
        cs: $ => $.cs_volcite,
        parameters: $ => [
          optional($.star),
          optional($.brack_group),
          $._text_token,
          optional($.brack_group),
          $._text_token
        ]
      },
      volcites: {
        cs: $ => $.cs_volcites,
        parameters: $ => [
          prec.right(-1,
            repeat1(
              seq(
                optional($.brack_group),
                $._text_token,
                optional($.brack_group),
                $._text_token
              )
            )
          )
        ]
      }
    }
  }
}
