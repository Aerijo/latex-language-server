# latex-language-server
Language server to provide various LaTeX tools

You're currently looking at the most complicated package ever to detect syntax errors in `.bib` files. More features are planned.

NOTE: 

- TODO: Convert all JSON receiving to SAX instead of DOM; have handlers provide a reader
- Text is stored in a `superstring` TextBuffer, and we treat UTF16 as a fixed width encoding. Any support of multibyte UTF16 will be purely coincidental.
- Initial plan was act like we're in a black box; new plan is use the file system like every other language server (and because the protocol has no spec on how to request files and directories). If someone wants it, they can add black box compatibility. I sure only plan on using it locally.
- Planned Features:
    - Locate and parse [CWL files](http://texstudio.sourceforge.net/manual/current/usermanual_en.html#CWLDESCRIPTION)
    - everything else
        - Seriously though, getting equivalent functionality of my existing Atom packages is top priority. This is basically
         - Static autocomplete
         - Word count
         - Biber linting
         - Hyperclick references, labels, etc
         - Citation autocomplete
         - Document structure (for folding and navigation tree)
        - Then dynamic autocomplete, compiling support, and anything else I can think of.
