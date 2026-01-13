# Error Handling in ADAN

## Purpose & Use cases
The purpose of the error API in ADAN is to collect, classify, and store diagnostics from compilation so they can be handled later.
Compiler stages (lexer,parser,semantic) report errors by providing:
- The source location and Length of the issue
- A specified severity level matching that type of problem
- A clear, user friendly message describing the issue
The error API only collects and stores errors, it does not format or handle them.

---

## What NOT to do
The error api is **not** used for:
- Compiler source debugging
- Runtime logging of compiler behavior
- Deciding if compiling should end
These should be handled by the individual systems

---

## Error Model

### Severity levels
Each severity level has its own purpose. Make sure you understand what information you wish the user to understand before choosing.
1. INFO     - For information messages that doesn't imply incorrect code, but to show the user the compilers behavior
2. WARNING  - For suggestions for best code practices (e.g. incorrect or unneeded type casing, unused variables, unneeded memory allocations)
3. ERROR    - For describing incorrect code which will break compilation (e.g. syntax errors and type errors)
4. CRITICAL - For cases where continuing the compilation sequence will not longer be reliable

---

### Categories
Errors are separated by category based on which stage of the compiler they originated:

- Lexer
- Parser
- Semantic
- Codegen
- Generics: Errors not associated with a specific compiler stage

### Error Ownership

The error list is created at the start of compilation by the compiler driver. It is owned by the driver and is passed to each compilation stage. The error list lives until the end of the compilation process and is destroyed by the compiler driver after compilation finishes.
