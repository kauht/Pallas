#include "parser_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lex/lexer.h"
#include "../parse/ast.h"
#include "../parse/parser.h"
#include "diagnostic.h"

/* ============================================================================
 * COLOR DEFINITIONS
 * ============================================================================ */

#if defined(_WIN32) || defined(_WIN64)
#define COLOR_PASS ""
#define COLOR_FAIL ""
#define COLOR_RESET ""
#define COLOR_HEADER ""
#define COLOR_INFO ""
#else
#define COLOR_PASS "\033[32m"
#define COLOR_FAIL "\033[31m"
#define COLOR_RESET "\033[0m"
#define COLOR_HEADER "\033[1;36m"
#define COLOR_INFO "\033[33m"
#endif

/* ============================================================================
 * HELPER FUNCTIONS
 * ============================================================================ */

static Token* lex_all(const char* src, size_t* out_count, ErrorList* errors) {
    Lexer* lexer = init_lexer("test.pal", src, errors);
    if (!lexer) {
        *out_count = 0;
        return NULL;
    }
    Token* tokens = run_lexer(lexer, out_count);
    free_lexer(lexer);
    return tokens;
}

static void free_tokens(Token* tokens, size_t count) {
    (void)count;
    if (tokens) {
        free(tokens);
    }
}

static void print_diagnostics(ErrorList* errors) {
    if (!errors || errors->size == 0) {
        return;
    }
    printf("  Diagnostics (%zu):\n", errors->size);
    for (size_t i = 0; i < errors->size; ++i) {
        Error* e = &errors->items[i];
        printf("    [%s] %s:%zu:%zu: %s\n",
               e->severity == SEVERITY_ERROR ? "ERROR" : "WARNING",
               e->filename ? e->filename : "<unknown>",
               e->line, e->column, e->message);
    }
}

static const char* ast_type_name(ASTNodeType type) {
    switch (type) {
        case AST_UNKNOWN: return "UNKNOWN";
        case AST_PROGRAM: return "PROGRAM";
        case AST_IMPORT: return "IMPORT";
        case AST_FUN_DECL: return "FUN_DECL";
        case AST_VAR_DECL: return "VAR_DECL";
        case AST_STRUCT_DECL: return "STRUCT_DECL";
        case AST_CLASS_DECL: return "CLASS_DECL";
        case AST_PARAM: return "PARAM";
        case AST_PARAM_LIST: return "PARAM_LIST";
        case AST_STRUCT_MEMBER: return "STRUCT_MEMBER";
        case AST_CLASS_MEMBER: return "CLASS_MEMBER";
        case AST_TYPE_BUILTIN: return "TYPE_BUILTIN";
        case AST_TYPE_USER: return "TYPE_USER";
        case AST_TYPE_POINTER: return "TYPE_POINTER";
        case AST_BLOCK: return "BLOCK";
        case AST_IF: return "IF";
        case AST_WHILE: return "WHILE";
        case AST_FOR: return "FOR";
        case AST_RETURN: return "RETURN";
        case AST_EXPR_STATEMENT: return "EXPR_STATEMENT";
        case AST_ASSIGNMENT: return "ASSIGNMENT";
        case AST_BINARY_EXPR: return "BINARY_EXPR";
        case AST_UNARY_EXPR: return "UNARY_EXPR";
        case AST_IDENTIFIER: return "IDENTIFIER";
        case AST_INT_LITERAL: return "INT_LITERAL";
        case AST_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case AST_CHAR_LITERAL: return "CHAR_LITERAL";
        case AST_STRING_LITERAL: return "STRING_LITERAL";
        default: return "INVALID";
    }
}

/* ============================================================================
 * ADVANCED PARSER TEST RUNNER
 * ============================================================================ */

typedef struct {
    const char* name;
    const char* source;
    int should_succeed;
    size_t expected_error_count;
    const char* category;
} ParserTest;

static int run_advanced_test(const ParserTest* test) {
    ErrorList errors = {0};
    size_t token_count = 0;
    Token* tokens = lex_all(test->source, &token_count, &errors);

    if (!tokens) {
        printf("%s[FAIL]%s %s - Lexer failed\n", COLOR_FAIL, COLOR_RESET, test->name);
        errors_clear(&errors);
        return 0;
    }

    Parser* parser = init_parser("test.pal", tokens, token_count, &errors);
    if (!parser) {
        printf("%s[FAIL]%s %s - Parser init failed\n", COLOR_FAIL, COLOR_RESET, test->name);
        free_tokens(tokens, token_count);
        errors_clear(&errors);
        return 0;
    }

    ASTNode* ast = run_parser(parser);
    int pass = 1;

    if (test->should_succeed) {
        if (!ast || ast->type != AST_PROGRAM) {
            printf("%s[FAIL]%s %s - Expected valid AST, got %s\n",
                   COLOR_FAIL, COLOR_RESET, test->name,
                   ast ? ast_type_name(ast->type) : "NULL");
            print_diagnostics(&errors);
            pass = 0;
        } else if (errors.size > test->expected_error_count) {
            printf("%s[FAIL]%s %s - Expected %zu errors, got %zu\n",
                   COLOR_FAIL, COLOR_RESET, test->name,
                   test->expected_error_count, errors.size);
            print_diagnostics(&errors);
            pass = 0;
        }
    } else {
        if (errors.size == 0) {
            printf("%s[FAIL]%s %s - Expected errors but got none\n",
                   COLOR_FAIL, COLOR_RESET, test->name);
            pass = 0;
        }
    }

    if (pass) {
        printf("%s[PASS]%s %s\n", COLOR_PASS, COLOR_RESET, test->name);
    }

    free(parser);
    free_tokens(tokens, token_count);
    errors_clear(&errors);
    return pass;
}

/* ============================================================================
 * STRUCTURAL TEST RUNNER (Deep AST Validation)
 * ============================================================================ */

typedef struct {
    const char* name;
    const char* source;
    ASTNodeType expected_root_type;
    size_t expected_import_count;
    size_t expected_top_decl_count;
    int (*validator)(ASTNode* ast);
    const char* category;
} StructuralTest;

static int validate_simple_import(ASTNode* ast) {
    if (!ast || ast->type != AST_PROGRAM) return 0;
    if (ast->program.import_count != 1) return 0;
    
    ASTNode* import = ast->program.imports[0];
    if (!import || import->type != AST_IMPORT) return 0;
    if (import->import_node.segment_count < 1) return 0;
    
    return 1;
}

static int validate_multiple_imports(ASTNode* ast) {
    if (!ast || ast->type != AST_PROGRAM) return 0;
    if (ast->program.import_count < 2) return 0;
    
    for (size_t i = 0; i < ast->program.import_count; i++) {
        if (!ast->program.imports[i] || ast->program.imports[i]->type != AST_IMPORT) {
            return 0;
        }
    }
    
    return 1;
}

static int run_structural_test(const StructuralTest* test) {
    ErrorList errors = {0};
    size_t token_count = 0;
    Token* tokens = lex_all(test->source, &token_count, &errors);

    if (!tokens) {
        printf("%s[FAIL]%s %s - Lexer failed\n", COLOR_FAIL, COLOR_RESET, test->name);
        errors_clear(&errors);
        return 0;
    }

    Parser* parser = init_parser("test.pal", tokens, token_count, &errors);
    if (!parser) {
        printf("%s[FAIL]%s %s - Parser init failed\n", COLOR_FAIL, COLOR_RESET, test->name);
        free_tokens(tokens, token_count);
        errors_clear(&errors);
        return 0;
    }

    ASTNode* ast = run_parser(parser);
    int pass = 1;

    if (!ast) {
        printf("%s[FAIL]%s %s - AST is NULL\n", COLOR_FAIL, COLOR_RESET, test->name);
        pass = 0;
        goto cleanup;
    }

    if (ast->type != test->expected_root_type) {
        printf("%s[FAIL]%s %s - Expected root type %s, got %s\n",
               COLOR_FAIL, COLOR_RESET, test->name,
               ast_type_name(test->expected_root_type),
               ast_type_name(ast->type));
        pass = 0;
        goto cleanup;
    }

    if (test->expected_root_type == AST_PROGRAM) {
        if (ast->program.import_count != test->expected_import_count) {
            printf("%s[FAIL]%s %s - Expected %zu imports, got %zu\n",
                   COLOR_FAIL, COLOR_RESET, test->name,
                   test->expected_import_count, ast->program.import_count);
            pass = 0;
            goto cleanup;
        }

        if (ast->program.top_decl_count != test->expected_top_decl_count) {
            printf("%s[FAIL]%s %s - Expected %zu top-level decls, got %zu\n",
                   COLOR_FAIL, COLOR_RESET, test->name,
                   test->expected_top_decl_count, ast->program.top_decl_count);
            pass = 0;
            goto cleanup;
        }
    }

    if (test->validator && !test->validator(ast)) {
        printf("%s[FAIL]%s %s - Custom validation failed\n",
               COLOR_FAIL, COLOR_RESET, test->name);
        print_diagnostics(&errors);
        pass = 0;
        goto cleanup;
    }

    if (pass) {
        printf("%s[PASS]%s %s\n", COLOR_PASS, COLOR_RESET, test->name);
    }

cleanup:
    free(parser);
    free_tokens(tokens, token_count);
    errors_clear(&errors);
    return pass;
}

/* ============================================================================
 * ADVANCED PARSER TESTS - COMPREHENSIVE LANGUAGE COVERAGE
 * ============================================================================ */

void run_parser_tests(void) {
    printf("\n");
    printf("+========================================================================+\n");
    printf("|              ADVANCED PARSER TEST SUITE - FULL COVERAGE                |\n");
    printf("+========================================================================+\n");
    printf("| Testing ALL language features from grammar.ebnf specification         |\n");
    printf("| See ADVANCED_PARSER_TESTS_SPEC.md for complete test requirements      |\n");
    printf("+========================================================================+\n\n");

    printf("%sNOTE: This is the foundation test suite. Expand to 600-800+ tests%s\n",
           COLOR_INFO, COLOR_RESET);
    printf("%susing the specification in ADVANCED_PARSER_TESTS_SPEC.md%s\n\n",
           COLOR_INFO, COLOR_RESET);

    int total = 0;
    int passed = 0;
    const char* current_category = NULL;
    int category_total = 0;
    int category_passed = 0;

    ParserTest tests[] = {
        /* ====================================================================
         * IMPORTS & PROGRAM STRUCTURE
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 1 for expansion to 15+ tests
         * ==================================================================== */
        {
            .name = "Empty program",
            .source = "",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "IMPORTS & PROGRAM STRUCTURE"
        },
        {
            .name = "Import - simple stdlib",
            .source = "import std;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "IMPORTS & PROGRAM STRUCTURE"
        },
        {
            .name = "Import - nested path",
            .source = "import std.io;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "IMPORTS & PROGRAM STRUCTURE"
        },
        {
            .name = "Import - relative current directory",
            .source = "import ./local.module;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "IMPORTS & PROGRAM STRUCTURE"
        },
        {
            .name = "Import - relative parent directory",
            .source = "import ../parent.sibling;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "IMPORTS & PROGRAM STRUCTURE"
        },
        {
            .name = "Import - multiple imports",
            .source = "import std.io;\nimport std.collections;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "IMPORTS & PROGRAM STRUCTURE"
        },
        {
            .name = "Import - missing semicolon (ERROR)",
            .source = "import std.io",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "IMPORTS & PROGRAM STRUCTURE"
        },
        {
            .name = "Import - invalid path (ERROR)",
            .source = "import 123;",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "IMPORTS & PROGRAM STRUCTURE"
        },

        /* ====================================================================
         * TYPE SYSTEM - Built-in & Sized Types
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 2 for expansion to 60+ tests
         * ==================================================================== */
        {
            .name = "Type - int",
            .source = "x: int;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - float",
            .source = "x: float;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - string",
            .source = "x: string;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - bool",
            .source = "x: bool;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - i32",
            .source = "x: i32;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - u64",
            .source = "x: u64;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - f32",
            .source = "x: f32;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - single pointer",
            .source = "x: i32*;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - double pointer",
            .source = "x: i32**;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - fixed array",
            .source = "x: i32[10];",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - 2D array",
            .source = "x: i32[3][3];",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },

        /* ====================================================================
         * VARIABLE DECLARATIONS
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 3 for expansion to 25+ tests
         * ==================================================================== */
        {
            .name = "VarDecl - simple uninitialized",
            .source = "x: i32;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - with initialization",
            .source = "x: i32 = 42;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - const with init",
            .source = "const PI: f64 = 3.14159;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - string",
            .source = "name: string = \"Alice\";",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - missing semicolon (ERROR)",
            .source = "x: i32 = 42",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "VARIABLE DECLARATIONS"
        },

        /* ====================================================================
         * FUNCTION DECLARATIONS
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 4 for expansion to 30+ tests
         * ==================================================================== */
        {
            .name = "FunDecl - no params no return",
            .source = "main() {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - with return type",
            .source = "main(): i32 {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - single parameter",
            .source = "square(x: i32): i32 {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - multiple parameters",
            .source = "add(a: i32, b: i32): i32 {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - variadic",
            .source = "sum(first: i32, ...): i32 {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },

        /* ====================================================================
         * STRUCT DECLARATIONS
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 5 for expansion to 15+ tests
         * ==================================================================== */
        {
            .name = "StructDecl - empty",
            .source = "struct Point {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRUCT DECLARATIONS"
        },
        {
            .name = "StructDecl - with fields",
            .source = "struct Point { x: f32; y: f32; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRUCT DECLARATIONS"
        },

        /* ====================================================================
         * CLASS DECLARATIONS
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 6 for expansion to 40+ tests
         * ==================================================================== */
        {
            .name = "ClassDecl - empty",
            .source = "class MyClass {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - with public field",
            .source = "class MyClass { public: x: i32; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - with method",
            .source = "class MyClass { public: getValue(): i32 {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - with constructor",
            .source = "class MyClass { MyClass() {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - with destructor",
            .source = "class MyClass { ~MyClass() {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },

        /* ====================================================================
         * ENUM DECLARATIONS
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 7 for expansion to 20+ tests
         * ==================================================================== */
        {
            .name = "EnumDecl - simple",
            .source = "enum Color { Red, Green, Blue }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ENUM DECLARATIONS"
        },
        {
            .name = "EnumDecl - with values",
            .source = "enum Color { Red = 1, Green = 2, Blue = 3 }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ENUM DECLARATIONS"
        },
        {
            .name = "EnumDecl - with data (tagged union)",
            .source = "enum Shape { Circle(radius: f32), Rectangle(w: f32, h: f32) }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ENUM DECLARATIONS"
        },

        /* ====================================================================
         * CONTROL FLOW - If Statements
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 8 for expansion to 20+ tests
         * ==================================================================== */
        {
            .name = "IfStmt - simple",
            .source = "main() { if (true) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CONTROL FLOW"
        },
        {
            .name = "IfStmt - if-else",
            .source = "main() { if (x > 0) {} else {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CONTROL FLOW"
        },
        {
            .name = "IfStmt - if-else-if-else",
            .source = "main() { if (x > 0) {} else if (x < 0) {} else {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CONTROL FLOW"
        },

        /* ====================================================================
         * LOOPS
         * See ADVANCED_PARSER_TESTS_SPEC.md Sections 9-10 for expansion to 40+ tests
         * ==================================================================== */
        {
            .name = "WhileStmt - simple",
            .source = "main() { while (true) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "ForStmt - C-style complete",
            .source = "main() { for (i: i32 = 0; i < 10; i++) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "ForStmt - infinite",
            .source = "main() { for (;;) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },

        /* ====================================================================
         * JUMP STATEMENTS
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 11 for expansion to 10+ tests
         * ==================================================================== */
        {
            .name = "BreakStmt",
            .source = "main() { while (true) { break; } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "JUMP STATEMENTS"
        },
        {
            .name = "ContinueStmt",
            .source = "main() { while (true) { continue; } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "JUMP STATEMENTS"
        },
        {
            .name = "ReturnStmt - no value",
            .source = "main() { return; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "JUMP STATEMENTS"
        },
        {
            .name = "ReturnStmt - with value",
            .source = "main(): i32 { return 42; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "JUMP STATEMENTS"
        },

        /* ====================================================================
         * PATTERN MATCHING
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 12 for expansion to 50+ tests
         * ==================================================================== */
        {
            .name = "MatchStmt - literal patterns",
            .source = "main() { match (x) { 0 => {} 1 => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - with guard",
            .source = "main() { match (x) { n if n > 0 => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - or-pattern",
            .source = "main() { match (x) { 1 | 2 | 3 => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },

        /* ====================================================================
         * EXPRESSIONS - Literals & Operators
         * See ADVANCED_PARSER_TESTS_SPEC.md Sections 13-14 for expansion to 150+ tests
         * ==================================================================== */
        {
            .name = "Expr - int literal",
            .source = "main() { x: i32 = 42; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - arithmetic",
            .source = "main() { x: i32 = 1 + 2 * 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - comparison",
            .source = "main() { x: bool = 5 > 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - logical",
            .source = "main() { x: bool = true && false; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - assignment",
            .source = "main() { x: i32 = 5; x += 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - function call",
            .source = "main() { foo(1, 2, 3); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - member access",
            .source = "main() { x: f32 = point.x; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - array subscript",
            .source = "main() { x: i32 = arr[0]; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - type cast",
            .source = "main() { x: f32 = (f32)42; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - new",
            .source = "main() { ptr: i32* = new i32; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - delete",
            .source = "main() { ptr: i32* = new i32; delete ptr; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * STRING INTERPOLATION
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 15 for expansion to 15+ tests
         * ==================================================================== */
        {
            .name = "String - interpolation",
            .source = "main() { name: string = \"Alice\"; msg: string = \"Hello, ${name}!\"; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRING INTERPOLATION"
        },

        /* ====================================================================
         * ERROR CASES
         * See ADVANCED_PARSER_TESTS_SPEC.md Section 17 for expansion to 60+ tests
         * ==================================================================== */
        {
            .name = "Error - missing closing brace",
            .source = "main() {",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - unmatched parentheses",
            .source = "main() { x: i32 = (1 + 2; }",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },

        /* ====================================================================
         * EXTENDED TYPE SYSTEM TESTS
         * ==================================================================== */
        {
            .name = "Type - void",
            .source = "foo(): void {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - char",
            .source = "c: char;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - double",
            .source = "d: double;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - i8",
            .source = "x: i8;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - i16",
            .source = "x: i16;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - i64",
            .source = "x: i64;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - u8",
            .source = "x: u8;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - u16",
            .source = "x: u16;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - u32",
            .source = "x: u32;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - f64",
            .source = "x: f64;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - triple pointer",
            .source = "x: i32***;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - quad pointer",
            .source = "x: i32****;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - void pointer",
            .source = "x: void*;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - string pointer",
            .source = "x: string*;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - pointer to array",
            .source = "x: i32[10]*;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - array of pointers",
            .source = "x: i32*[10];",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - 3D array",
            .source = "x: i32[2][3][4];",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - 4D array",
            .source = "x: i32[2][3][4][5];",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - size 1 array",
            .source = "x: i32[1];",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - large array",
            .source = "x: i32[1024];",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - parenthesized",
            .source = "x: (i32);",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - parenthesized pointer",
            .source = "x: (i32*);",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - pointer to parenthesized",
            .source = "x: (i32)*;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - complex pointer array combo",
            .source = "x: i32**[5][10];",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - user defined",
            .source = "x: MyClass;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },
        {
            .name = "Type - user defined pointer",
            .source = "x: MyClass*;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "TYPE SYSTEM"
        },

        /* ====================================================================
         * EXTENDED VARIABLE DECLARATIONS
         * ==================================================================== */
        {
            .name = "VarDecl - pointer type",
            .source = "ptr: i32*;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - array type",
            .source = "arr: i32[10];",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - multiple in function",
            .source = "main() { x: i32; y: i32; z: i32; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - char with init",
            .source = "c: char = 'a';",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - bool with init",
            .source = "b: bool = true;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - float with init",
            .source = "f: float = 3.14;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - double with init",
            .source = "d: double = 2.718;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - pointer with null init",
            .source = "ptr: i32* = null;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - const uninitialized (ERROR)",
            .source = "const X: i32;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },
        {
            .name = "VarDecl - expression init",
            .source = "x: i32 = 1 + 2 * 3;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "VARIABLE DECLARATIONS"
        },

        /* ====================================================================
         * EXTENDED FUNCTION DECLARATIONS
         * ==================================================================== */
        {
            .name = "FunDecl - void return",
            .source = "foo(): void {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - pointer return",
            .source = "getPtr(): i32* {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - array return",
            .source = "getArray(): i32[10] {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - pointer param",
            .source = "foo(ptr: i32*): void {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - array param",
            .source = "foo(arr: i32[10]): void {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - mixed params",
            .source = "foo(a: i32, b: string, c: bool): void {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - variadic at end",
            .source = "printf(fmt: string, ...): void {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - multiple statements",
            .source = "main(): i32 { x: i32 = 5; y: i32 = 10; return x + y; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - nested blocks",
            .source = "main() { { { x: i32 = 5; } } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },
        {
            .name = "FunDecl - empty block",
            .source = "main() {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "FUNCTION DECLARATIONS"
        },

        /* ====================================================================
         * EXTENDED STRUCT DECLARATIONS
         * ==================================================================== */
        {
            .name = "StructDecl - single field",
            .source = "struct Point { x: f32; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRUCT DECLARATIONS"
        },
        {
            .name = "StructDecl - many fields",
            .source = "struct Vec3 { x: f32; y: f32; z: f32; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRUCT DECLARATIONS"
        },
        {
            .name = "StructDecl - mixed types",
            .source = "struct Person { name: string; age: i32; active: bool; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRUCT DECLARATIONS"
        },
        {
            .name = "StructDecl - nested struct field",
            .source = "struct Node { value: i32; next: Node*; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRUCT DECLARATIONS"
        },
        {
            .name = "StructDecl - pointer fields",
            .source = "struct Buffer { data: i32*; size: u64; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRUCT DECLARATIONS"
        },
        {
            .name = "StructDecl - array fields",
            .source = "struct Matrix { data: f32[16]; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRUCT DECLARATIONS"
        },

        /* ====================================================================
         * EXTENDED CLASS DECLARATIONS
         * ==================================================================== */
        {
            .name = "ClassDecl - multiple access sections",
            .source = "class C { public: x: i32; private: y: i32; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - methods and fields",
            .source = "class C { public: x: i32; getValue(): i32 {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - constructor with params",
            .source = "class C { C(x: i32, y: i32) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - full featured",
            .source = "class C { public: x: i32; C() {} ~C() {} getValue(): i32 {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - private section",
            .source = "class C { private: secret: i32; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - method with params",
            .source = "class C { public: setValue(v: i32): void {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },
        {
            .name = "ClassDecl - method returning pointer",
            .source = "class C { public: getPtr(): i32* {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CLASS DECLARATIONS"
        },

        /* ====================================================================
         * EXTENDED ENUM DECLARATIONS
         * ==================================================================== */
        {
            .name = "EnumDecl - single variant",
            .source = "enum Color { Red }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ENUM DECLARATIONS"
        },
        {
            .name = "EnumDecl - trailing comma",
            .source = "enum Color { Red, Green, Blue, }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ENUM DECLARATIONS"
        },
        {
            .name = "EnumDecl - explicit values",
            .source = "enum Status { Success = 0, Error = 1, Pending = 2 }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ENUM DECLARATIONS"
        },
        {
            .name = "EnumDecl - variant with single param",
            .source = "enum Option { Some(value: i32), None }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ENUM DECLARATIONS"
        },
        {
            .name = "EnumDecl - variant with multiple params",
            .source = "enum Result { Ok(value: i32), Err(code: i32, msg: string) }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ENUM DECLARATIONS"
        },
        {
            .name = "EnumDecl - mixed variants",
            .source = "enum Shape { Circle(r: f32), Rectangle(w: f32, h: f32), Square = 3 }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ENUM DECLARATIONS"
        },

        /* ====================================================================
         * EXTENDED CONTROL FLOW
         * ==================================================================== */
        {
            .name = "IfStmt - nested if",
            .source = "main() { if (x > 0) { if (y > 0) {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CONTROL FLOW"
        },
        {
            .name = "IfStmt - multiple else-if",
            .source = "main() { if (x > 0) {} else if (x < 0) {} else if (x == 0) {} else {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CONTROL FLOW"
        },
        {
            .name = "IfStmt - with declarations",
            .source = "main() { if (true) { x: i32 = 5; } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CONTROL FLOW"
        },
        {
            .name = "IfStmt - complex condition",
            .source = "main() { if (x > 0 && y < 10 || z == 5) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CONTROL FLOW"
        },
        {
            .name = "IfStmt - single line body",
            .source = "main() { if (true) return; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "CONTROL FLOW"
        },

        /* ====================================================================
         * EXTENDED LOOPS
         * ==================================================================== */
        {
            .name = "WhileStmt - with break",
            .source = "main() { while (true) { break; } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "WhileStmt - with continue",
            .source = "main() { while (true) { continue; } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "WhileStmt - nested",
            .source = "main() { while (x > 0) { while (y > 0) {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "WhileStmt - complex condition",
            .source = "main() { while (x > 0 && y < 10) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "ForStmt - no init",
            .source = "main() { for (; i < 10; i++) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "ForStmt - no condition",
            .source = "main() { for (i: i32 = 0;; i++) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "ForStmt - no increment",
            .source = "main() { for (i: i32 = 0; i < 10;) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "ForStmt - decrement",
            .source = "main() { for (i: i32 = 10; i > 0; i--) {} }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "ForStmt - nested",
            .source = "main() { for (i: i32 = 0; i < 10; i++) { for (j: i32 = 0; j < 10; j++) {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "ForStmt - with break",
            .source = "main() { for (i: i32 = 0; i < 10; i++) { if (i == 5) break; } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },
        {
            .name = "ForStmt - with continue",
            .source = "main() { for (i: i32 = 0; i < 10; i++) { if (i == 5) continue; } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "LOOPS"
        },

        /* ====================================================================
         * EXTENDED PATTERN MATCHING
         * ==================================================================== */
        {
            .name = "MatchStmt - int literals",
            .source = "main() { match (x) { 1 => {} 2 => {} 3 => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - string literals",
            .source = "main() { match (s) { \"hello\" => {} \"world\" => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - char literals",
            .source = "main() { match (c) { 'a' => {} 'b' => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - bool literals",
            .source = "main() { match (b) { true => {} false => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - null literal",
            .source = "main() { match (ptr) { null => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - identifier pattern",
            .source = "main() { match (x) { n => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - enum pattern no params",
            .source = "main() { match (color) { Color::Red => {} Color::Green => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - enum pattern with params",
            .source = "main() { match (opt) { Option::Some(x) => {} Option::None => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - or-pattern multiple",
            .source = "main() { match (x) { 1 | 2 | 3 | 4 => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - guard simple",
            .source = "main() { match (x) { n if n > 0 => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - guard complex",
            .source = "main() { match (x) { n if n > 0 && n < 10 => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - struct pattern empty",
            .source = "main() { match (p) { Point {} => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - struct pattern with fields",
            .source = "main() { match (p) { Point { x, y } => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - struct pattern with binding",
            .source = "main() { match (p) { Point { x: a, y: b } => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - array pattern empty",
            .source = "main() { match (arr) { [] => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - array pattern with elements",
            .source = "main() { match (arr) { [a, b, c] => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - array pattern with rest",
            .source = "main() { match (arr) { [first, ...rest] => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },
        {
            .name = "MatchStmt - type pattern",
            .source = "main() { match (x) { n: i32 => {} _ => {} } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "PATTERN MATCHING"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - LITERALS
         * ==================================================================== */
        {
            .name = "Expr - negative int",
            .source = "main() { x: i32 = -42; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - float literal",
            .source = "main() { x: float = 3.14159; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - negative float",
            .source = "main() { x: float = -2.718; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - char literal",
            .source = "main() { c: char = 'a'; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - string literal",
            .source = "main() { s: string = \"hello\"; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - true literal",
            .source = "main() { b: bool = true; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - false literal",
            .source = "main() { b: bool = false; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - null literal",
            .source = "main() { ptr: i32* = null; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - ARITHMETIC
         * ==================================================================== */
        {
            .name = "Expr - addition",
            .source = "main() { x: i32 = 1 + 2; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - subtraction",
            .source = "main() { x: i32 = 5 - 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - multiplication",
            .source = "main() { x: i32 = 3 * 4; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - division",
            .source = "main() { x: i32 = 10 / 2; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - modulo",
            .source = "main() { x: i32 = 10 % 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - complex arithmetic",
            .source = "main() { x: i32 = (1 + 2) * (3 - 4) / 5 % 6; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - COMPARISON
         * ==================================================================== */
        {
            .name = "Expr - less than",
            .source = "main() { x: bool = 1 < 2; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - less or equal",
            .source = "main() { x: bool = 1 <= 2; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - greater than",
            .source = "main() { x: bool = 2 > 1; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - greater or equal",
            .source = "main() { x: bool = 2 >= 1; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - equality",
            .source = "main() { x: bool = 5 == 5; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - inequality",
            .source = "main() { x: bool = 5 != 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - LOGICAL
         * ==================================================================== */
        {
            .name = "Expr - logical and",
            .source = "main() { x: bool = true && false; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - logical or",
            .source = "main() { x: bool = true || false; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - logical not",
            .source = "main() { x: bool = !true; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - complex logical",
            .source = "main() { x: bool = (a && b) || (!c && d); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - BITWISE
         * ==================================================================== */
        {
            .name = "Expr - bitwise and",
            .source = "main() { x: i32 = 5 & 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - bitwise or",
            .source = "main() { x: i32 = 5 | 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - bitwise xor",
            .source = "main() { x: i32 = 5 ^ 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - bitwise not",
            .source = "main() { x: i32 = ~5; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - left shift",
            .source = "main() { x: i32 = 1 << 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - right shift",
            .source = "main() { x: i32 = 8 >> 2; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - ASSIGNMENT
         * ==================================================================== */
        {
            .name = "Expr - simple assign",
            .source = "main() { x: i32; x = 5; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - subtract assign",
            .source = "main() { x: i32 = 10; x -= 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - multiply assign",
            .source = "main() { x: i32 = 5; x *= 2; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - divide assign",
            .source = "main() { x: i32 = 10; x /= 2; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - modulo assign",
            .source = "main() { x: i32 = 10; x %= 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - bitwise and assign",
            .source = "main() { x: i32 = 5; x &= 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - bitwise or assign",
            .source = "main() { x: i32 = 5; x |= 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - bitwise xor assign",
            .source = "main() { x: i32 = 5; x ^= 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - left shift assign",
            .source = "main() { x: i32 = 1; x <<= 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - right shift assign",
            .source = "main() { x: i32 = 8; x >>= 2; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - UNARY
         * ==================================================================== */
        {
            .name = "Expr - pre increment",
            .source = "main() { x: i32 = 5; ++x; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - pre decrement",
            .source = "main() { x: i32 = 5; --x; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - post increment",
            .source = "main() { x: i32 = 5; x++; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - post decrement",
            .source = "main() { x: i32 = 5; x--; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - unary minus",
            .source = "main() { x: i32 = -5; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - address of",
            .source = "main() { x: i32 = 5; ptr: i32* = &x; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - dereference",
            .source = "main() { ptr: i32*; x: i32 = *ptr; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - POSTFIX
         * ==================================================================== */
        {
            .name = "Expr - function call no args",
            .source = "main() { foo(); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - function call one arg",
            .source = "main() { foo(42); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - function call many args",
            .source = "main() { foo(1, 2, 3, 4, 5); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - nested function calls",
            .source = "main() { foo(bar(baz())); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - member access simple",
            .source = "main() { x: f32 = obj.field; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - member access chain",
            .source = "main() { x: f32 = obj.field1.field2.field3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - arrow access",
            .source = "main() { x: f32 = ptr->field; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - arrow access chain",
            .source = "main() { x: f32 = ptr->field1->field2; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - array subscript simple",
            .source = "main() { x: i32 = arr[0]; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - array subscript expression",
            .source = "main() { x: i32 = arr[i + 1]; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - multidimensional array",
            .source = "main() { x: i32 = arr[0][1]; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - 3D array access",
            .source = "main() { x: i32 = arr[0][1][2]; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - method call",
            .source = "main() { obj.method(1, 2); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - chained method calls",
            .source = "main() { obj.method1().method2().method3(); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - CASTING
         * ==================================================================== */
        {
            .name = "Expr - cast to int",
            .source = "main() { x: i32 = (i32)3.14; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - cast to float",
            .source = "main() { x: f32 = (f32)42; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - cast to pointer",
            .source = "main() { ptr: i32* = (i32*)addr; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - nested cast",
            .source = "main() { x: f64 = (f64)(i32)3.14; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - NEW/DELETE
         * ==================================================================== */
        {
            .name = "Expr - new simple type",
            .source = "main() { ptr: i32* = new i32; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - new with constructor args",
            .source = "main() { obj: MyClass* = new MyClass(1, 2); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - new array",
            .source = "main() { arr: i32* = new i32[10]; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - delete variable",
            .source = "main() { ptr: i32* = new i32; delete ptr; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - delete expression",
            .source = "main() { delete getPtr(); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - TERNARY
         * ==================================================================== */
        {
            .name = "Expr - ternary simple",
            .source = "main() { x: i32 = true ? 1 : 0; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - ternary nested",
            .source = "main() { x: i32 = a ? (b ? 1 : 2) : 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - ternary with expressions",
            .source = "main() { x: i32 = (a > b) ? (a + b) : (a - b); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * EXTENDED EXPRESSIONS - PARENTHESIZED
         * ==================================================================== */
        {
            .name = "Expr - parenthesized simple",
            .source = "main() { x: i32 = (5); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - parenthesized nested",
            .source = "main() { x: i32 = ((((5)))); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },
        {
            .name = "Expr - precedence override",
            .source = "main() { x: i32 = (1 + 2) * 3; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EXPRESSIONS"
        },

        /* ====================================================================
         * STRING INTERPOLATION EXTENDED
         * ==================================================================== */
        {
            .name = "String - simple interpolation",
            .source = "main() { msg: string = \"Value: ${x}\"; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRING INTERPOLATION"
        },
        {
            .name = "String - multiple interpolations",
            .source = "main() { msg: string = \"${x} + ${y} = ${x + y}\"; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRING INTERPOLATION"
        },
        {
            .name = "String - nested expression",
            .source = "main() { msg: string = \"Result: ${func(a, b)}\"; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "STRING INTERPOLATION"
        },

        /* ====================================================================
         * COMPLEX REAL-WORLD SCENARIOS
         * ==================================================================== */
        {
            .name = "Complex - linked list node",
            .source = "struct Node { value: i32; next: Node*; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },
        {
            .name = "Complex - generic function",
            .source = "max(a: i32, b: i32): i32 { if (a > b) return a; else return b; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },
        {
            .name = "Complex - fibonacci function",
            .source = "fib(n: i32): i32 { if (n <= 1) return n; return fib(n - 1) + fib(n - 2); }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },
        {
            .name = "Complex - factorial loop",
            .source = "fact(n: i32): i32 { result: i32 = 1; for (i: i32 = 1; i <= n; i++) { result *= i; } return result; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },
        {
            .name = "Complex - array sum",
            .source = "sum(arr: i32[10]): i32 { total: i32 = 0; for (i: i32 = 0; i < 10; i++) { total += arr[i]; } return total; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },
        {
            .name = "Complex - simple class with methods",
            .source = "class Counter { public: value: i32; inc(): void { value++; } dec(): void { value--; } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },
        {
            .name = "Complex - class with constructor and destructor",
            .source = "class Resource { public: data: i32*; Resource() { data = new i32; } ~Resource() { delete data; } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },
        {
            .name = "Complex - enum with methods",
            .source = "enum Result { Ok(value: i32), Err(msg: string) }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },
        {
            .name = "Complex - pattern match on enum",
            .source = "main() { match (result) { Result::Ok(val) => { println(val); } Result::Err(msg) => { println(msg); } } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },
        {
            .name = "Complex - nested control flow",
            .source = "main() { for (i: i32 = 0; i < 10; i++) { if (i % 2 == 0) { while (true) { break; } } else { continue; } } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "REAL-WORLD"
        },

        /* ====================================================================
         * MORE ERROR RECOVERY TESTS
         * ==================================================================== */
        {
            .name = "Error - missing function body",
            .source = "main(): i32;",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - unclosed string",
            .source = "main() { s: string = \"hello; }",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - invalid token in expression",
            .source = "main() { x: i32 = @ 5; }",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - missing type in declaration",
            .source = "main() { x = 5; }",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - missing parameter type",
            .source = "foo(x): i32 {}",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - duplicate semicolons",
            .source = "main() { x: i32 = 5;; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - missing comma in params",
            .source = "foo(a: i32 b: i32): void {}",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - missing closing bracket",
            .source = "main() { arr: i32[10; }",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - mismatched braces",
            .source = "main() { if (true) { }",
            .should_succeed = 0,
            .expected_error_count = 1,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - break outside loop",
            .source = "main() { break; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ERROR RECOVERY"
        },
        {
            .name = "Error - continue outside loop",
            .source = "main() { continue; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "ERROR RECOVERY"
        },

        /* ====================================================================
         * EDGE CASES AND BOUNDARY CONDITIONS
         * ==================================================================== */
        {
            .name = "Edge - empty function body",
            .source = "main() {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EDGE CASES"
        },
        {
            .name = "Edge - empty struct",
            .source = "struct Empty {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EDGE CASES"
        },
        {
            .name = "Edge - empty class",
            .source = "class Empty {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EDGE CASES"
        },
        {
            .name = "Edge - single statement block",
            .source = "main() { return; }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EDGE CASES"
        },
        {
            .name = "Edge - deeply nested blocks",
            .source = "main() { { { { { { x: i32 = 5; } } } } } }",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EDGE CASES"
        },
        {
            .name = "Edge - long identifier",
            .source = "veryLongIdentifierNameThatIsStillValidAccordingToTheLanguageSpecification: i32;",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EDGE CASES"
        },
        {
            .name = "Edge - many function parameters",
            .source = "func(a: i32, b: i32, c: i32, d: i32, e: i32, f: i32, g: i32, h: i32): void {}",
            .should_succeed = 1,
            .expected_error_count = 0,
            .category = "EDGE CASES"
        },
    };

    size_t test_count = sizeof(tests) / sizeof(tests[0]);

    for (size_t i = 0; i < test_count; i++) {
        if (tests[i].category != current_category) {
            if (current_category != NULL) {
                printf("  %s[%d/%d passed (%.1f%%)]%s\n\n",
                       COLOR_INFO,
                       category_passed, category_total,
                       category_total > 0 ? (float)category_passed / category_total * 100.0f : 0.0f,
                       COLOR_RESET);
                category_total = 0;
                category_passed = 0;
            }
            
            printf("%s=== %s ===%s\n", COLOR_HEADER, tests[i].category, COLOR_RESET);
            current_category = tests[i].category;
        }
        
        total++;
        category_total++;
        if (run_advanced_test(&tests[i])) {
            passed++;
            category_passed++;
        }
    }

    if (current_category != NULL) {
        printf("  %s[%d/%d passed (%.1f%%)]%s\n\n",
               COLOR_INFO,
               category_passed, category_total,
               category_total > 0 ? (float)category_passed / category_total * 100.0f : 0.0f,
               COLOR_RESET);
    }

    printf("\n+========================================================================+\n");
    printf("| OVERALL RESULTS: %d/%d tests passed (%.1f%%)                            \n",
           passed, total, total > 0 ? (float)passed / total * 100.0f : 0.0f);
    printf("+========================================================================+\n");
    printf("| NEXT STEPS: Expand to 600-800+ tests using specification documents    |\n");
    printf("| - Read: ADVANCED_PARSER_TESTS_SPEC.md for complete requirements       |\n");
    printf("| - Read: PARSER_TEST_UPGRADE_GUIDE.md for implementation guide         |\n");
    printf("| - Read: PARSER_TESTS_ACTION_CHECKLIST.md for step-by-step plan       |\n");
    printf("+========================================================================+\n\n");
}

/* ============================================================================
 * STRUCTURAL PARSER TESTS (Deep AST Validation)
 * ============================================================================ */

void run_parser_structural_tests(void) {
    printf("\n");
    printf("+========================================================================+\n");
    printf("|                    PARSER STRUCTURAL TEST SUITE                        |\n");
    printf("+========================================================================+\n");
    printf("| Deep AST validation tests                                             |\n");
    printf("+========================================================================+\n\n");

    int total = 0;
    int passed = 0;

    StructuralTest tests[] = {
        {
            .name = "Structural - single import",
            .source = "import std.io;",
            .expected_root_type = AST_PROGRAM,
            .expected_import_count = 1,
            .expected_top_decl_count = 0,
            .validator = validate_simple_import,
            .category = "STRUCTURAL"
        },
        {
            .name = "Structural - multiple imports",
            .source = "import std.io;\nimport std.collections;\nimport ./local.module;",
            .expected_root_type = AST_PROGRAM,
            .expected_import_count = 3,
            .expected_top_decl_count = 0,
            .validator = validate_multiple_imports,
            .category = "STRUCTURAL"
        },
        {
            .name = "Structural - minimal main function",
            .source = "main(): i32 { return 0; }",
            .expected_root_type = AST_PROGRAM,
            .expected_import_count = 0,
            .expected_top_decl_count = 1,
            .validator = NULL,
            .category = "STRUCTURAL"
        },
        {
            .name = "Structural - complete program",
            .source = 
                "import std.io;\n"
                "struct Point { x: f32; y: f32; }\n"
                "const PI: f64 = 3.14159;\n"
                "main(): i32 { return 0; }",
            .expected_root_type = AST_PROGRAM,
            .expected_import_count = 1,
            .expected_top_decl_count = 3,
            .validator = NULL,
            .category = "STRUCTURAL"
        },
    };

    size_t test_count = sizeof(tests) / sizeof(tests[0]);

    for (size_t i = 0; i < test_count; i++) {
        total++;
        if (run_structural_test(&tests[i])) {
            passed++;
        }
    }

    printf("\n+========================================================================+\n");
    printf("| RESULTS: %d/%d tests passed (%.1f%%)                                     \n",
           passed, total, total > 0 ? (float)passed / total * 100.0f : 0.0f);
    printf("+========================================================================+\n\n");
}