module.exports = {
  common: {
    commands: {
      lua: {
        cs: $ => $.cs_lua,
        parameters: $ => [optional($._number), $._apply_parameter]
      }
    }
  }
}
