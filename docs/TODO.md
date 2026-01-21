# Pallas TODO

## MAX PRIORITY

### Parser Implementation
- Implement expression parser with correct precedence (assignment → logical OR → logical AND → equality → comparison → additive → multiplicative → unary → primary)
- Implement statement parser (if/else, while, for, return, break, continue, blocks)
- Implement declaration parser (functions, classes, structs, enums, variables)
- Implement type parser with suffix handling (pointers, arrays)
- Implement pattern matching parser for match statements
- Build complete AST nodes for all language constructs
- Add parser error recovery and synchronization
- Create comprehensive parser test suite

### Type System
- Implement type checking system with no implicit casting enforcement
- Handle complex pointer types (int**, int***, etc.)
- Handle array types with size expressions
- Implement type equivalence checking
- Add type checking for pattern matches and ensure exhaustiveness
- Support enum variant type validation

### Semantic Analysis
- Build symbol table with scope management
- Implement identifier and type resolution
- Check function signatures and validate overloading
- Validate constructor/destructor pairs
- Enforce access control (public/private)
- Validate enum variant usage in patterns

---

## HIGH PRIORITY

### Pattern Matching
- Implement pattern compilation to efficient match code
- Generate exhaustiveness checking for match statements
- Handle guard clauses in patterns
- Support destructuring for enums and structs
- Optimize pattern matching code generation

### String Interpolation
- Parse ${expression} within string literals in lexer/parser
- Generate code that evaluates and formats interpolated expressions
- Handle nested braces and escape sequences correctly

### Enums & Tagged Unions
- Implement enum type checking for simple and data-carrying enums
- Generate efficient enum representation (tagged unions)
- Implement enum variant constructors
- Support pattern matching on enum variants

### Memory Management
- Implement `new` operator for heap allocation (single objects and arrays)
- Implement `delete` operator with automatic destructor calls
- Call destructors automatically on scope exit for stack objects
- Add optional memory leak detection in debug mode

### Object-Oriented Features
- Implement class member layout and memory alignment
- Generate constructor and destructor code with proper initialization order
- Implement method dispatch and `this` pointer handling
- Support member access via `.` and `->`

### Control Flow & Functions
- Implement break/continue statements
- Validate control flow for unreachable code detection
- Handle variadic functions (`...`)
- Generate function call code with proper calling conventions
- Check that all return paths return values for non-void functions

---

## MEDIUM PRIORITY

### Error Messages & Diagnostics
- Improve error messages with context, suggestions, and source snippets
- Add warning system for unused variables, unused return values, etc.
- Show helpful diagnostics with "did you mean?" suggestions
- Implement multiple error reporting (don't stop at first error)

### Testing
- Add parser tests covering all grammar rules and edge cases
- Add semantic analysis tests for type checking and scoping
- Create integration tests for complete programs
- Build benchmark suite for lexer, parser, and compilation speed

### Documentation
- Write parser implementation documentation
- Write semantic analysis documentation
- Document code generation process
- Create language tutorial with examples
- Create API reference for standard library
- Add more example programs showcasing language features

---

## LOW PRIORITY

### Code Generation & Backend
- Design intermediate representation (IR)
- Choose backend strategy (LLVM, custom backend, or C code generation)
- Implement IR generation from AST
- Generate target code with proper linking
- Create executable output

### Optimization
- Dead code elimination
- Constant folding and propagation
- Inline expansion for small functions
- Loop optimizations (unrolling, strength reduction)
- Tail call optimization

### Standard Library
- Design stdlib API and module structure
- Implement I/O module (std/io): println, print, file I/O
- Implement collections module: dynamic arrays, hash maps, sets
- Implement string utilities module
- Implement math library with common functions

### Module System
- Implement module resolution algorithm
- Handle relative imports (`./`, `../`) and absolute imports (`std/`)
- Detect circular dependencies
- Support separate compilation and linking
- Generate object files per module

---

## FUTURE CONSIDERATIONS

### Language Design Decisions (Not Yet Decided)
- Error handling strategy: exceptions vs Result types vs Go-style multiple returns
- Generics syntax: `Array<T>` vs `Array[T]` or other notation
- Lambda expression syntax and closure semantics
- Inheritance support and syntax for classes
- Traits/interfaces for polymorphism
- Operator overloading permissions and restrictions
- Move semantics and ownership tracking
- Compile-time execution and const functions

### Advanced Language Features (Long-term)
- Generics/templates with type parameters
- Lambda expressions with capture support
- Range-based for loops (`for (i in 0..10)`)
- Traits or interfaces for abstract types
- Move semantics for efficient resource transfer
- Compile-time function evaluation

### Platform Support
- Linux support
- macOS support
- Cross-compilation support
- WebAssembly target

### Developer Tools
- Language Server Protocol (LSP) implementation for IDE support
- Debugger integration (DWARF/PDB generation)
- Documentation generator from source comments
- Package manager design

---

## KNOWN ISSUES

- Parser: Stub implementation with many unimplemented functions
- String interpolation: Lexer doesn't yet tokenize ${...} specially
- Grammar: Need runtime verification for remaining ambiguities
- No semantic analysis or code generation yet

---
