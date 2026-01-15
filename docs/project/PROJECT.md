# Project Plan

## Roadmap

1. Language design (syntax, types, design decisions) **COMPLETED**
2. Core (lexer, parser, semantic, codegen) **IN PROGRESS**
3. Type system and other features (pointers, classes, lib development, etc.)
4. Advanced features (templates, optimization, advanced libs, etc.)
5. Platform support

## Current Status

### Completed
- [x] Language syntax design
- [x] Type system design (int, float, char, string, bool, sized types)
- [x] Lexer implementation with full token support
- [x] Comprehensive lexer test suite (38 tests)
- [x] Diagnostic/error reporting system
- [x] Basic grammar specification (EBNF)
- [x] Class/struct syntax
- [x] Constructor/destructor syntax
- [x] Visibility modifiers (public:/private:)
- [x] Import/include system syntax
- [x] Variadic function syntax
- [x] Memory management syntax (new/delete)
- [x] Operator precedence design
- [x] Control flow structures (if/else, for, while, break, continue)

### In Progress
- [ ] Parser implementation
  - [ ] Expression parsing with correct precedence
  - [ ] Statement parsing
  - [ ] Declaration parsing
  - [ ] Type parsing
- [ ] Semantic analysis
  - [ ] Symbol table
  - [ ] Type checking
  - [ ] Scope resolution
- [ ] AST construction

### TODO - Core Features

#### Parser & Semantic
- [ ] Complete parser for all grammar rules
- [ ] AST node definitions for all constructs
- [ ] Symbol table implementation
- [ ] Type checking system
- [ ] Scope management
- [ ] Function overloading resolution
- [ ] Constructor/destructor validation
- [ ] Access control enforcement (public:/private:)

#### Code Generation
- [ ] IR design and implementation
- [ ] Code generation backend
- [ ] Target platform selection (LLVM? custom?)
- [ ] Optimization passes

#### Type System
- [ ] Pointer arithmetic and safety
- [ ] Array implementation
- [ ] Type inference (where applicable)
- [ ] Generic/template system design
- [ ] No implicit casting enforcement

#### Object-Oriented Features
- [ ] Class member layout
- [ ] Method dispatch
- [ ] Constructor/destructor calling conventions
- [ ] Inheritance (if/when added)
- [ ] Virtual functions (if/when added)

#### Memory Management
- [ ] Allocation/deallocation implementation
- [ ] Stack vs heap management
- [ ] Destructor automatic calling
- [ ] Memory leak detection (optional/debug mode)

### TODO - Advanced Features

#### Language Features
- [ ] Generics/templates design and implementation
- [ ] Lambda expressions
- [ ] Pattern matching/switch statements
- [ ] Range-based for loops (`for (i in 0..10)`)
- [ ] Iterator-based for loops
- [ ] String formatting system
- [ ] Error handling mechanism

#### Standard Library
- [ ] stdlib API design
- [ ] I/O library (println, print, file operations)
- [ ] Collections (dynamic arrays, maps, sets)
- [ ] String utilities
- [ ] Math library
- [ ] Memory utilities

#### Module System
- [ ] Module resolution algorithm
- [ ] Import path handling
- [ ] Cyclic dependency detection
- [ ] Separate compilation support
- [ ] Package management design

#### Tooling & Developer Experience
- [ ] Error messages with context and suggestions
- [ ] Warning system
- [ ] Unused variable/value detection
- [ ] Return value handling warnings
- [ ] Documentation generator
- [ ] Language server protocol (LSP) support
- [ ] Debugger integration

#### Optimization & Performance
- [ ] Basic optimization passes
- [ ] Dead code elimination
- [ ] Constant folding
- [ ] Inline expansion
- [ ] Advanced optimization (if needed)

#### Platform Support
- [ ] Windows (primary target)
- [ ] Linux
- [ ] macOS
- [ ] Cross-compilation support

## Design Decisions to Finalize

### Syntax Decisions
- [x] Variable declaration: `name: type = value`
- [x] Function declaration: `name(params): returnType { }`
- [x] Class syntax with public:/private:
- [x] Constructor: same name as class
- [x] Destructor: `~ClassName()`
- [x] Import: `import module.submodule;` or `include module.file;`

### Implementation Decisions Needed
- [ ] **For loop variations**: Which syntax to support?
  - C-style: `for (i: i32 = 0; i < 10; i++)` (decided)
  - Range: `for (i in 0..10)`
  - Iterator: `for (item : array)`
- [ ] **Pattern matching**: Do we want match/switch? What syntax?
- [ ] **Error handling**: Exceptions? Result types? Go-style errors?
- [ ] **Generics syntax**: `Array<T>` or `Array[T]` or other?
- [ ] **Lambda syntax**: `(x) => x * 2` or `|x| { x * 2 }` or other?
- [ ] **Inheritance**: Do we support class inheritance? If so, what syntax?
- [ ] **Interfaces/Traits**: Do we need abstract interfaces?
- [ ] **Operator overloading**: Should classes be able to overload operators?

## Questions to Resolve

1. **Memory model**: Pure manual (like C) or with some safety features?
2. **Null safety**: Should we have optional types or keep nullable pointers?
3. **String type**: Mutable or immutable by default?
4. **Collections ownership**: Move semantics or copy semantics?
5. **Compilation model**: Ahead-of-time only or JIT support?
6. **Standard library**: Minimal (like C) or comprehensive (like Python)?

## Testing Strategy

### Current Tests
- Lexer: 38 comprehensive unit tests
- Token stream printing with diagnostics

### Needed Tests
- [ ] Parser tests for all grammar rules
- [ ] Semantic analysis tests
- [ ] Type checking tests
- [ ] Code generation tests
- [ ] Integration tests (full programs)
- [ ] Standard library tests
- [ ] Benchmark suite

## Project Structure

```
pallas/
├── docs/          # Documentation
├── examples/      # Example programs
├── include/       # Public headers
├── src/
│   ├── lex/      # Lexer implementation
│   ├── parse/    # Parser (in progress)
│   ├── semantic/ # Semantic analysis (todo)
│   ├── ir/       # Intermediate representation (todo)
│   ├── codegen/  # Code generation (todo)
│   └── tests/    # Test suite
└── stdlib/        # Standard library (todo)
```

## Timeline Estimate

- **Phase 1**: Parser + AST (1-2 months)
- **Phase 2**: Semantic Analysis (1-2 months)
- **Phase 3**: Code Generation (2-3 months)
- **Phase 4**: Standard Library (1-2 months)
- **Phase 5**: Advanced Features (3-6 months)
- **Phase 6**: Optimization & Polish (ongoing)

**Total estimated time**: 8-15 months for working compiler with basic stdlib

---

**Last Updated**: December 2024

**Status**: Early development - lexer complete, parser next
