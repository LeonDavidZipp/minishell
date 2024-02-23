## 1. Character Classification
The lexer reads the input characters one by one and classifies them into groups (like letters, digits, whitespace, special characters, etc.).

## 2. Token Recognition
Based on the sequence of characters, the lexer recognizes them as valid tokens in the language. For example, it might recognize "int" as a keyword, "x" as an identifier, "123" as a numeric literal, and so on.

## 3. Token Generation
For each recognized token, the lexer generates a token object. This object typically contains the type of the token and the lexeme (the exact string of characters that make up the token).

## 4. Whitespace and Comment Stripping
As part of token generation, the lexer often discards whitespace and comments, as they're usually not needed for the later stages of compilation.

## 5. Error Handling
If the lexer encounters a sequence of characters that doesn't form a valid token (like an ill-formed number or an unrecognized symbol), it generates an error.
