# Lexical Analysis in Pallas

## Purpose
The purpose of lexical analysis in Pallas is to take in the source text and convert it into a stream of tokens to use for later stages. The lexer is only responsible for classifying and location tracking, not validating the programs structure.

---

## Input and Output
### Input
The lexer takes in the source as `const char*` provided by the compiler driver. The lexer doesn't do any I/O and doesn't change the source.
### Output
The lexer outputs tokens in order, each token should include:
- Token Type
- Location (line/column)
- Length (in source)
- Literal value (if literal token)

---

## Ownership
- The lexer doesn't own the source
- Every token `lexeme` is a pointer to the original source buffer using start and length

---

## Lexer Lifetime
The Lexer, along with all other major stages of the compiler, are created and destroyed by the compiler driver, each stage is created and destroyed in order one after another.

## Non-Goals
- No matching braces/parens
- No detecting type errors
- No parsing
- No AST creation
- No semantic analysis
- No printing

## Errors
When lexer gets an invalid input, it:
- Emits a TOKEN_ERROR
- Records the error's location
- Consumes at least 1 character
- Continues Lexing

## API
**`next_char`** - Advances `pos` and updates line/column
**`next_token`** - Returns a valid token (including `TOKEN_ERROR`, and `TOKEN_EOF`)
**`literal`** - Memory must be freed, `lexeme` should not be freed

### Notes
- Whitespace, Newlines and Comments(including `//` and `/* */`) are skipped (Unterminated comments produce TOKEN_ERROR)
- Identifiers start with a letter or `_`, and can be followed by letters, digits, or `_`'s
- **After** scanning an identifier, check it against the keyword list. If it matches, send the specified keyword token, otherwise send TOKEN_IDENT

#### Numbers:
- Integers: sequence of just digits (0, 9) = TOKEN_INT
- Floating point: digits with either a `.` = TOKEN_FLOAT
- Invalid numbers(like `154...24`) = TOKEN_ERROR

#### Strings/Characters
- String: `"xxx"`
- Char: `'x'`
- Supported escape sequences: `\n`, `\t`, `\\`, `\'`, `\"`
- Unterminated string/char = TOKEN_ERROR

#### Operators/Punctuation
- Operators with multiple characters should be matched longest-shortest, like `++` before `+`
- ++,--,==,!=,<=,>=,&&,||,<<,>>,->,... | +,-,*,/,%,=,!,&,|,^,~,<,>

#### Line/Column tracking
- Column increments for each char
- Line increments on `\n`, then column resets to 1
- `next_char` should always update both correctly

#### Errors
- Unexpected characters = TOKEN_ERROR
- Records error location (line/column)
