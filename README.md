# latex-language-server
Language server to provide various LaTeX tools

NOTE: 

- TODO: Convert all JSON receiving to SAX instead of DOM; have handlers provide a reader
- Text will be stored in a `superstring` TextBuffer, and we will treat UTF16 as a fixed width encoding. Any support of multibyte UTF16 will be purely coincidental.
- Initial plan was act like we're in a black box; new plan is use the file system like every other language server (and because the protocol has no spec on how to request files and directories). If someone wants it, they can add black box compatibility.
- Planned Features:
    - Locate and parse [CWL files](http://texstudio.sourceforge.net/manual/current/usermanual_en.html#CWLDESCRIPTION)
