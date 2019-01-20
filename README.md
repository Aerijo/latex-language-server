# latex-language-server
Language server to provide various LaTeX tools

NOTE: 

- We will initially treat the server as working in a black box, only able to communicate with the client. We can later add direct filesystem support.
- Text will be stored in a `superstring` TextBuffer, and we will treat UTF16 as a fixed width encoding. Any support of multibyte UTF16 will be purely coincidental.
