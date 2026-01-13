# Syntax Analysis(Parsing) in ADAN

## Purpose
The purpose of Syntax Analysis is to detect syntax errors and verify the grammatical structure of the program, then build an Abstract Syntax Tree from the provided Token list for further processing.
---

## Input and Output
### Input
The parser takes in a Token list as a Token*
### Output
The parser returns an Abstract Syntax Tree
---

## Ownership
- The parser doesn't own the source or the list of Tokens
- Every token `lexeme` is a pointer to the original source buffer using start and length
- Non literal ASTNode's don't allocate any memory

---

## Parser Lifetime
The Parser, along with all other major stages of the compiler, are created and destroyed by the compiler driver, each stage is created and destroyed in order one after another.

---

## Non-Goals

## Errors
The parser should produce a complete AST, even when errors are present.

## API
**`xxx`** - Advances `xxx` and updates line/column


### Notes




#### Errors
- Records error location (line/column) of lexeme
