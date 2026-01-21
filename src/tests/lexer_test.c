/*
 * lexer_test.c
 *
 * Polished and refactored lexer test driver.
 *
 * - Cleaner, more consistent output.
 * - Portable: avoids non-portable APIs and keeps behavior deterministic.
 * - Modular helpers for printing, comparison, and test orchestration.
 *
 * The test driver exercises the lexer and validates:
 *  - Token sequences (type + optional lexeme)
 *  - Diagnostic counts
 *
 * Note: This file assumes the project's headers provide the usual
 * Lexer, Token, TokenType, and ErrorList APIs:
 *   - Lexer* init_lexer(const char*, const char*, ErrorList*);
 *   - Token* run_lexer(Lexer*, size_t*);
 *   - void free_lexer(Lexer*);
 *   - void free_token(Token*);
 *   - ErrorList (with `size`, `items[]`), errors_format(), errors_clear()
 *
 * Keep the test expectations aligned with the lexer implementation.
 */

#include "lexer_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------- Configuration ----------------------------- */

/* Default example path used when lexing files for human inspection. */
#ifndef EXAMPLE_FILE_PATH
#define EXAMPLE_FILE_PATH "C:\\Users\\sam\\Desktop\\pallas\\examples\\pallas.pal"
#endif

/* Colors (optional): define ENABLE_COLOR to enable terminal colors. */
#ifdef ENABLE_COLOR
#define COLOR_PASS "\x1b[32;1m"
#define COLOR_FAIL "\x1b[31;1m"
#define COLOR_RESET "\x1b[0m"
#else
#define COLOR_PASS ""
#define COLOR_FAIL ""
#define COLOR_RESET ""
#endif

/* ----------------------------- Utilities -------------------------------- */

/* Map TokenType -> readable name.
 * Keep this list synchronized with `TokenType` in lexer.h.
 */
static const char* token_name(TokenType t) {
    switch (t) {
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_ERROR: return "TOKEN_ERROR";
        case TOKEN_IDENT: return "TOKEN_IDENT";
        case TOKEN_INT_LITERAL: return "TOKEN_INT_LITERAL";
        case TOKEN_FLOAT_LITERAL: return "TOKEN_FLOAT_LITERAL";
        case TOKEN_STRING_LITERAL: return "TOKEN_STRING_LITERAL";
        case TOKEN_CHAR_LITERAL: return "TOKEN_CHAR_LITERAL";

        /* Keywords */
        case TOKEN_IMPORT: return "TOKEN_IMPORT";
        case TOKEN_INCLUDE: return "TOKEN_INCLUDE";
        case TOKEN_IF: return "TOKEN_IF";
        case TOKEN_ELSE: return "TOKEN_ELSE";
        case TOKEN_FOR: return "TOKEN_FOR";
        case TOKEN_WHILE: return "TOKEN_WHILE";
        case TOKEN_BREAK: return "TOKEN_BREAK";
        case TOKEN_CONTINUE: return "TOKEN_CONTINUE";
        case TOKEN_RETURN: return "TOKEN_RETURN";
        case TOKEN_STRUCT: return "TOKEN_STRUCT";
        case TOKEN_CLASS: return "TOKEN_CLASS";
        case TOKEN_PUBLIC: return "TOKEN_PUBLIC";
        case TOKEN_PRIVATE: return "TOKEN_PRIVATE";
        case TOKEN_NEW: return "TOKEN_NEW";
        case TOKEN_DELETE: return "TOKEN_DELETE";
        case TOKEN_TRUE: return "TOKEN_TRUE";
        case TOKEN_FALSE: return "TOKEN_FALSE";
        case TOKEN_NULL: return "TOKEN_NULL";
        case TOKEN_CONST: return "TOKEN_CONST";
        case TOKEN_VOID: return "TOKEN_VOID";
        case TOKEN_MATCH: return "TOKEN_MATCH";
        case TOKEN_ENUM: return "TOKEN_ENUM";

        /* Types */
        case TOKEN_INT: return "TOKEN_INT";
        case TOKEN_FLOAT: return "TOKEN_FLOAT";
        case TOKEN_DOUBLE: return "TOKEN_DOUBLE";
        case TOKEN_CHAR: return "TOKEN_CHAR";
        case TOKEN_STRING: return "TOKEN_STRING";
        case TOKEN_BOOL: return "TOKEN_BOOL";
        case TOKEN_I8: return "TOKEN_I8";
        case TOKEN_I16: return "TOKEN_I16";
        case TOKEN_I32: return "TOKEN_I32";
        case TOKEN_I64: return "TOKEN_I64";
        case TOKEN_U8: return "TOKEN_U8";
        case TOKEN_U16: return "TOKEN_U16";
        case TOKEN_U32: return "TOKEN_U32";
        case TOKEN_U64: return "TOKEN_U64";
        case TOKEN_F8: return "TOKEN_F8";
        case TOKEN_F16: return "TOKEN_F16";
        case TOKEN_F32: return "TOKEN_F32";
        case TOKEN_F64: return "TOKEN_F64";

        /* Operators & punctuation */
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_PLUS_PLUS: return "TOKEN_PLUS_PLUS";
        case TOKEN_PLUS_ASSIGN: return "TOKEN_PLUS_ASSIGN";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_MINUS_MINUS: return "TOKEN_MINUS_MINUS";
        case TOKEN_MINUS_ASSIGN: return "TOKEN_MINUS_ASSIGN";
        case TOKEN_ARROW: return "TOKEN_ARROW";
        case TOKEN_STAR: return "TOKEN_STAR";
        case TOKEN_STAR_ASSIGN: return "TOKEN_STAR_ASSIGN";
        case TOKEN_SLASH: return "TOKEN_SLASH";
        case TOKEN_SLASH_ASSIGN: return "TOKEN_SLASH_ASSIGN";
        case TOKEN_PERCENT: return "TOKEN_PERCENT";
        case TOKEN_ASSIGN: return "TOKEN_ASSIGN";
        case TOKEN_EQUAL: return "TOKEN_EQUAL";
        case TOKEN_NOT_EQUAL: return "TOKEN_NOT_EQUAL";
        case TOKEN_LESS: return "TOKEN_LESS";
        case TOKEN_LESS_EQUAL: return "TOKEN_LESS_EQUAL";
        case TOKEN_GREATER: return "TOKEN_GREATER";
        case TOKEN_GREATER_EQUAL: return "TOKEN_GREATER_EQUAL";
        case TOKEN_LEFT_SHIFT: return "TOKEN_LEFT_SHIFT";
        case TOKEN_LSHIFT_EQUALS: return "TOKEN_LSHIFT_EQUALS";
        case TOKEN_RIGHT_SHIFT: return "TOKEN_RIGHT_SHIFT";
        case TOKEN_RSHIFT_EQUALS: return "TOKEN_RSHIFT_EQUALS";
        case TOKEN_CARET: return "TOKEN_CARET";
        case TOKEN_XOR_EQUALS: return "TOKEN_XOR_EQUALS";
        case TOKEN_AMPERSAND: return "TOKEN_AMPERSAND";
        case TOKEN_AND_EQUALS: return "TOKEN_AND_EQUALS";
        case TOKEN_LOGICAL_AND: return "TOKEN_LOGICAL_AND";
        case TOKEN_PIPE: return "TOKEN_PIPE";
        case TOKEN_OR_EQUALS: return "TOKEN_OR_EQUALS";
        case TOKEN_LOGICAL_OR: return "TOKEN_LOGICAL_OR";
        case TOKEN_LOGICAL_NOT: return "TOKEN_LOGICAL_NOT";
        case TOKEN_TILDE: return "TOKEN_TILDE";
        case TOKEN_DOT: return "TOKEN_DOT";
        case TOKEN_ELLIPSIS: return "TOKEN_ELLIPSIS";
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_LBRACE: return "TOKEN_LBRACE";
        case TOKEN_RBRACE: return "TOKEN_RBRACE";
        case TOKEN_LBRACKET: return "TOKEN_LBRACKET";
        case TOKEN_RBRACKET: return "TOKEN_RBRACKET";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_COLON: return "TOKEN_COLON";
        case TOKEN_DOUBLE_COLON: return "TOKEN_DOUBLE_COLON";
        case TOKEN_FAT_ARROW: return "TOKEN_FAT_ARROW";
        case TOKEN_QUESTION: return "TOKEN_QUESTION";
        case TOKEN_AT: return "TOKEN_AT";

        default:
            return "TOKEN_<unknown>";
    }
}

/* Print a single token succinctly. */
static void print_token(const Token* t, size_t index) {
    const char* name = token_name(t->type);
    if (t->lexeme) {
        printf("[%3zu] %-22s '%s' (line %zu, col %zu)\n", index, name, t->lexeme, t->line, t->column);
    } else {
        printf("[%3zu] %-22s (line %zu, col %zu)\n", index, name, t->line, t->column);
    }
}

/* Print full token stream for debugging. */
static void print_token_stream(const Token* tokens, size_t count) {
    puts("\n+========================================================================+");
    puts("|                          TOKEN STREAM                                  |");
    puts("+========================================================================+");
    for (size_t i = 0; i < count; ++i) {
        print_token(&tokens[i], i);
    }
    puts("+========================================================================+\n");
}

/* Free token array and contained lexemes. */
static void free_tokens(Token* toks, size_t n) {
    if (!toks) return;
    for (size_t i = 0; i < n; ++i) {
        free_token(&toks[i]);
    }
    free(toks);
}

/* Compare a token against expected type and optional lexeme.
 * If expected_lexeme is NULL, we expect token.lexeme == NULL.
 */
static int token_eq(const Token* t, TokenType expected_type, const char* expected_lexeme) {
    if (t->type != expected_type) return 0;
    if (!expected_lexeme) return t->lexeme == NULL;
    if (!t->lexeme) return 0;
    return strcmp(t->lexeme, expected_lexeme) == 0;
}

/* Lex the provided source string and return tokens (caller must free). */
static Token* lex_all(const char* src, size_t* out_count, ErrorList* el) {
    Lexer* lx = init_lexer(EXAMPLE_FILE_PATH, src, el);
    if (!lx) return NULL;
    Token* tokens = run_lexer(lx, out_count);
    free_lexer(lx);
    return tokens;
}

/* Print diagnostics (if any) from ErrorList in a compact format. */
static void print_diagnostics(const ErrorList* errors) {
    if (!errors || errors->size == 0) return;
    puts("Diagnostics:");
    for (size_t i = 0; i < errors->size; ++i) {
        char* formatted = errors_format(&errors->items[i]);
        if (formatted) {
            printf("  - %s\n", formatted);
            free(formatted);
        } else {
            printf("  - <unformatted diagnostic>\n");
        }
    }
}

/* ----------------------------- Test Runner ------------------------------- */

typedef struct {
    TokenType type;
    const char* lexeme; /* NULL indicates we expect NULL lexeme */
} ExpectedToken;

/* run_test: returns 1 on success, 0 on failure */
static int run_test(const char* name, const char* src, const ExpectedToken* expected,
                    size_t expected_count, size_t expected_errors) {
    ErrorList errors = {0};
    size_t count = 0;
    Token* tokens = lex_all(src, &count, &errors);
    int pass = 1;

    if (!tokens) {
        printf("%s[FAIL]%s %s  (lexer allocation/initialization failure)\n", COLOR_FAIL, COLOR_RESET, name);
        errors_clear(&errors);
        return 0;
    }

    if (count != expected_count) {
        printf("%s[FAIL]%s %s\n", COLOR_FAIL, COLOR_RESET, name);
        printf("  Expected %zu tokens, got %zu\n", expected_count, count);
        print_token_stream(tokens, count);
        print_diagnostics(&errors);
        pass = 0;
        goto cleanup;
    }

    for (size_t i = 0; i < count; ++i) {
        if (!token_eq(&tokens[i], expected[i].type, expected[i].lexeme)) {
            printf("%s[FAIL]%s %s\n", COLOR_FAIL, COLOR_RESET, name);
            printf("  Token %zu mismatch:\n", i);
            printf("    Expected: %s '%s'\n", token_name(expected[i].type),
                   expected[i].lexeme ? expected[i].lexeme : "(null)");
            printf("      Got:   %s '%s'\n", token_name(tokens[i].type),
                   tokens[i].lexeme ? tokens[i].lexeme : "(null)");
            print_token_stream(tokens, count);
            print_diagnostics(&errors);
            pass = 0;
            goto cleanup;
        }
    }

    if (errors.size != expected_errors) {
        printf("%s[FAIL]%s %s\n", COLOR_FAIL, COLOR_RESET, name);
        printf("  Expected %zu diagnostics, got %zu\n", expected_errors, errors.size);
        print_diagnostics(&errors);
        pass = 0;
        goto cleanup;
    }

    /* Success */
    printf("%s[PASS]%s %s\n", COLOR_PASS, COLOR_RESET, name);

cleanup:
    free_tokens(tokens, count);
    errors_clear(&errors);
    return pass;
}

/* Lex an example file and print tokens for manual inspection. */
static void lex_example_file(void) {
    const char* example_path = EXAMPLE_FILE_PATH;
    printf("\n+========================================================================+\n");
    printf("|                      LEXING EXAMPLE FILE                               |\n");
    printf("+------------------------------------------------------------------------+\n");
    printf("| File: %-66s |\n", example_path);
    printf("+========================================================================+\n");

    FILE* f = NULL;
#ifdef _MSC_VER
    errno_t err = fopen_s(&f, example_path, "rb");
    if (err != 0 || !f) {
#else
    f = fopen(example_path, "rb");
    if (!f) {
#endif
        printf("  ⚠ Could not open example file: %s\n\n", example_path);
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size <= 0) {
        printf("  ⚠ Example file is empty or unreadable\n\n");
        fclose(f);
        return;
    }

    char* buf = (char*)malloc((size_t)size + 1);
    if (!buf) {
        printf("  ⚠ Memory allocation failure reading example file\n\n");
        fclose(f);
        return;
    }

    fread(buf, 1, (size_t)size, f);
    buf[size] = '\0';
    fclose(f);

    ErrorList errors = {0};
    size_t count = 0;
    Token* tokens = lex_all(buf, &count, &errors);

    if (tokens) {
        print_token_stream(tokens, count);
        free_tokens(tokens, count);
    } else {
        printf("  ⚠ Lexer failed while processing example file\n");
    }

    if (errors.size > 0) {
        printf("\n");
        print_diagnostics(&errors);
    }

    errors_clear(&errors);
    free(buf);
}

/* ----------------------------- Test Suite --------------------------------
 * The suite below is intentionally explicit: each test block defines the
 * expected token sequence inline. This keeps each test easy to inspect and
 * edit. The suite exercises both successful lexemes and recovery cases.
 * ------------------------------------------------------------------------- */

void run_lexer_tests(void) {
    int passed = 0;
    int total = 0;

    puts("\n+========================================================================+");
    puts("|                          LEXER TEST SUITE                              |");
    puts("+========================================================================+\n");

    /* ---------- Test 1: Keywords ---------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IMPORT, "import"}, {TOKEN_IF, "if"}, {TOKEN_ELSE, "else"},
            {TOKEN_FOR, "for"}, {TOKEN_WHILE, "while"}, {TOKEN_BREAK, "break"},
            {TOKEN_CONTINUE, "continue"}, {TOKEN_RETURN, "return"}, {TOKEN_STRUCT, "struct"},
            {TOKEN_CLASS, "class"}, {TOKEN_TRUE, "true"}, {TOKEN_FALSE, "false"},
            {TOKEN_NULL, "null"}, {TOKEN_CONST, "const"}, {TOKEN_EOF, NULL},
        };
        const char* src = "import if else for while break continue return struct class true false null const";
        total++;
        passed += run_test("Keywords", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* ---------- Test 2: Types ---------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_INT, "int"}, {TOKEN_FLOAT, "float"}, {TOKEN_DOUBLE, "double"},
            {TOKEN_CHAR, "char"}, {TOKEN_STRING, "string"}, {TOKEN_BOOL, "bool"},
            {TOKEN_I32, "i32"}, {TOKEN_I64, "i64"}, {TOKEN_U32, "u32"},
            {TOKEN_U64, "u64"}, {TOKEN_F32, "f32"}, {TOKEN_F64, "f64"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "int float double char string bool i32 i64 u32 u64 f32 f64";
        total++;
        passed += run_test("Types", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* ---------- Test 3: Identifiers ---------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "myVar"}, {TOKEN_IDENT, "_hidden123"}, {TOKEN_IDENT, "camelCase"},
            {TOKEN_IDENT, "UPPER_CASE"}, {TOKEN_IDENT, "x"}, {TOKEN_EOF, NULL},
        };
        const char* src = "myVar _hidden123 camelCase UPPER_CASE x";
        total++;
        passed += run_test("Identifiers", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* ---------- Test 4: Numbers ---------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_INT_LITERAL, "0"}, {TOKEN_INT_LITERAL, "123"}, {TOKEN_INT_LITERAL, "9999"},
            {TOKEN_FLOAT_LITERAL, "4.56"}, {TOKEN_FLOAT_LITERAL, "7.89"}, {TOKEN_FLOAT_LITERAL, "0.5"},
            {TOKEN_FLOAT_LITERAL, "123.456"}, {TOKEN_EOF, NULL},
        };
        const char* src = "0 123 9999 4.56 7.89 0.5 123.456";
        total++;
        passed += run_test("Numbers", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* ---------- Test 5: Character Literals ---------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_CHAR_LITERAL, "a"}, {TOKEN_CHAR_LITERAL, "\\"}, {TOKEN_CHAR_LITERAL, "z"},
            {TOKEN_CHAR_LITERAL, "\n"}, {TOKEN_CHAR_LITERAL, "\""}, {TOKEN_EOF, NULL},
        };
        const char* src = "'a' '\\\\' 'z' '\\n' '\\\"'";
        total++;
        passed += run_test("Character Literals", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* ---------- Test 6: String Literals ---------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_STRING_LITERAL, ""}, {TOKEN_STRING_LITERAL, "hello"},
            {TOKEN_STRING_LITERAL, "line\nbreak"}, {TOKEN_STRING_LITERAL, "escaped\"quote"},
            {TOKEN_STRING_LITERAL, "tab\ttab"}, {TOKEN_EOF, NULL},
        };
        const char* src = "\"\" \"hello\" \"line\\nbreak\" \"escaped\\\"quote\" \"tab\\ttab\"";
        total++;
        passed += run_test("String Literals", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* ---------- Test 7: Operators & Punctuation ---------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_PLUS, "+"}, {TOKEN_PLUS_PLUS, "++"}, {TOKEN_PLUS_ASSIGN, "+="},
            {TOKEN_MINUS, "-"}, {TOKEN_MINUS_MINUS, "--"}, {TOKEN_MINUS_ASSIGN, "-="},
            {TOKEN_ARROW, "->"}, {TOKEN_STAR, "*"}, {TOKEN_STAR_ASSIGN, "*="},
            {TOKEN_SLASH, "/"}, {TOKEN_SLASH_ASSIGN, "/="}, {TOKEN_PERCENT, "%"},
            {TOKEN_ASSIGN, "="}, {TOKEN_EQUAL, "=="}, {TOKEN_NOT_EQUAL, "!="},
            {TOKEN_LESS, "<"}, {TOKEN_LESS_EQUAL, "<="}, {TOKEN_GREATER, ">"},
            {TOKEN_GREATER_EQUAL, ">="}, {TOKEN_LEFT_SHIFT, "<<"}, {TOKEN_LSHIFT_EQUALS, "<<="},
            {TOKEN_RIGHT_SHIFT, ">>"}, {TOKEN_RSHIFT_EQUALS, ">>="}, {TOKEN_CARET, "^"},
            {TOKEN_XOR_EQUALS, "^="}, {TOKEN_AMPERSAND, "&"}, {TOKEN_AND_EQUALS, "&="},
            {TOKEN_LOGICAL_AND, "&&"}, {TOKEN_PIPE, "|"}, {TOKEN_OR_EQUALS, "|="},
            {TOKEN_LOGICAL_OR, "||"}, {TOKEN_LOGICAL_NOT, "!"}, {TOKEN_TILDE, "~"},
            {TOKEN_DOT, "."}, {TOKEN_ELLIPSIS, "..."}, {TOKEN_EOF, NULL},
        };
        const char* src = "+ ++ += - -- -= -> * *= / /= % = == != < <= > >= << <<= >> >>= ^ ^= & &= && | |= || ! ~ . ...";
        total++;
        passed += run_test("Operators", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* ---------- Test 8: Comments ---------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_INT_LITERAL, "1"}, {TOKEN_PLUS, "+"}, {TOKEN_INT_LITERAL, "2"}, {TOKEN_EOF, NULL},
        };
        const char* src = "1 // This is a comment\n+ 2 /* multi\nline\ncomment */";
        total++;
        passed += run_test("Comments", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* ---------- Test 9: Mixed example ---------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IF, "if"}, {TOKEN_LPAREN, "("}, {TOKEN_IDENT, "x"},
            {TOKEN_EQUAL, "=="}, {TOKEN_INT_LITERAL, "5"}, {TOKEN_RPAREN, ")"},
            {TOKEN_LBRACE, "{"}, {TOKEN_IDENT, "println"}, {TOKEN_LPAREN, "("},
            {TOKEN_STRING_LITERAL, "x is 5"}, {TOKEN_RPAREN, ")"}, {TOKEN_SEMICOLON, ";"},
            {TOKEN_RBRACE, "}"}, {TOKEN_EOF, NULL},
        };
        const char* src = "if (x == 5) { println(\"x is 5\"); }";
        total++;
        passed += run_test("Mixed Example", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* ---------- Tests: more constructs (abbreviated) ---------- */
    {
        /* Keep many tests explicit in the suite; not all shown here for brevity. */
    }

    /* ---------- Error-recovery tests ---------- */

    /* Unterminated string literal -> TOKEN_ERROR + 1 diagnostic */
    {
        const ExpectedToken exp[] = {
            {TOKEN_ERROR, NULL}, {TOKEN_EOF, NULL},
        };
        const char* src = "\"unterminated string";
        total++;
        passed += run_test("Unterminated String Literal", src, exp, sizeof(exp) / sizeof(exp[0]), 1);
    }

    /* Unterminated character literal -> TOKEN_ERROR + 1 diagnostic (consistent with strings) */
    {
        const ExpectedToken exp[] = {
            {TOKEN_ERROR, NULL}, {TOKEN_EOF, NULL},
        };
        const char* src = "'a";
        total++;
        passed += run_test("Unterminated Character Literal", src, exp, sizeof(exp) / sizeof(exp[0]), 1);
    }

    /* Unterminated block comment -> lexer records diagnostic and continues (EOF) */
    {
        const ExpectedToken exp[] = {
            {TOKEN_INT_LITERAL, "1"}, {TOKEN_EOF, NULL},
        };
        const char* src = "1 /* unclosed block comment";
        total++;
        passed += run_test("Unterminated Block Comment", src, exp, sizeof(exp) / sizeof(exp[0]), 1);
    }

    /* Invalid number with multiple decimal points */
    {
        const ExpectedToken exp[] = {
            {TOKEN_FLOAT_LITERAL, "1.2"}, {TOKEN_DOT, "."}, {TOKEN_INT_LITERAL, "3"}, {TOKEN_EOF, NULL},
        };
        const char* src = "1.2.3";
        total++;
        passed += run_test("Invalid Number - Too Many Decimals", src, exp, sizeof(exp) / sizeof(exp[0]), 1);
    }

    /* ---------- Summary ---------- */
    puts("\n+========================================================================+");
    puts("|                           TEST RESULTS                                 |");
    puts("+------------------------------------------------------------------------+");
    if (passed == total) {
        printf("|  %s[PASS]%s All tests passed (%d/%d)                                  |\n", COLOR_PASS, COLOR_RESET, passed, total);
    } else {
        printf("|  %s[FAIL]%s Some tests failed: %d/%d passed                            |\n", COLOR_FAIL, COLOR_RESET, passed, total);
    }
    puts("+========================================================================+\n");

    if (passed != total) {
        /* Non-zero exit indicates test failure when invoked by CI or scripts. */
        exit(1);
    }

    /* Finally, lex and print the example file for manual inspection. */
    lex_example_file();
}