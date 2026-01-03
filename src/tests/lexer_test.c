#include "lexer_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- utilities ---------- */

static const char* token_name(TokenType t) {
    switch (t) {
        case TOKEN_EOF:
            return "TOKEN_EOF";
        case TOKEN_ERROR:
            return "TOKEN_ERROR";
        case TOKEN_IDENT:
            return "TOKEN_IDENT";
        case TOKEN_INT_LITERAL:
            return "TOKEN_INT_LITERAL";
        case TOKEN_FLOAT_LITERAL:
            return "TOKEN_FLOAT_LITERAL";
        case TOKEN_STRING_LITERAL:
            return "TOKEN_STRING_LITERAL";
        case TOKEN_CHAR_LITERAL:
            return "TOKEN_CHAR_LITERAL";

        case TOKEN_IF:
            return "TOKEN_IF";
        case TOKEN_ELSE:
            return "TOKEN_ELSE";
        case TOKEN_RETURN:
            return "TOKEN_RETURN";
        case TOKEN_CONST:
            return "TOKEN_CONST";

        case TOKEN_PLUS:
            return "TOKEN_PLUS";
        case TOKEN_PLUS_PLUS:
            return "TOKEN_PLUS_PLUS";
        case TOKEN_PLUS_ASSIGN:
            return "TOKEN_PLUS_ASSIGN";

        case TOKEN_MINUS:
            return "TOKEN_MINUS";
        case TOKEN_MINUS_MINUS:
            return "TOKEN_MINUS_MINUS";
        case TOKEN_ARROW:
            return "TOKEN_ARROW";

        case TOKEN_LOGICAL_AND:
            return "TOKEN_LOGICAL_AND";
        case TOKEN_LOGICAL_OR:
            return "TOKEN_LOGICAL_OR";

        case TOKEN_ASSIGN:
            return "TOKEN_ASSIGN";

        default:
            return "TOKEN_<unknown>";
    }
}

static int token_eq(Token* t, TokenType type, const char* lexeme) {
    if (t->type != type) {
        return 0;
    }
    if (lexeme == NULL) {
        return t->lexeme == NULL;
    }
    if (t->lexeme == NULL) {
        return 0;
    }
    return strcmp(t->lexeme, lexeme) == 0;
}

static Token* lex_all(const char* src, size_t* out_count, ErrorList* el) {
    Lexer* lx = init_lexer(src, el);

    size_t cap = 16;
    size_t n = 0;
    Token* toks = malloc(sizeof(Token) * cap);

    for (;;) {
        if (n == cap) {
            cap *= 2;
            toks = realloc(toks, sizeof(Token) * cap);
        }
        Token t = next_token(lx);
        toks[n++] = t;
        if (t.type == TOKEN_EOF) {
            break;
        }
    }

    free_lexer(lx);
    *out_count = n;
    return toks;
}

static void free_tokens(Token* toks, size_t n) {
    for (size_t i = 0; i < n; i++) {
        free_token(&toks[i]);
    }
    free(toks);
}

/* ---------- test runner ---------- */

typedef struct {
    TokenType type;
    const char* lexeme;
} ExpectedToken;

static int run_test(const char* name, const char* src, const ExpectedToken* expected,
                    size_t expected_count, size_t expected_errors) {
    ErrorList errors = {0};
    size_t count = 0;

    Token* tokens = lex_all(src, &count, &errors);

    int pass = 1;

    if (count != expected_count) {
        printf("[FAIL] %s\n", name);
        printf("  Expected %zu tokens, got %zu\n", expected_count, count);
        pass = 0;
        goto done;
    }

    for (size_t i = 0; i < count; i++) {
        if (!token_eq(&tokens[i], expected[i].type, expected[i].lexeme)) {
            printf("[FAIL] %s\n", name);
            printf("  Token %zu:\n", i);
            printf("    Expected: %s '%s'\n", token_name(expected[i].type),
                   expected[i].lexeme ? expected[i].lexeme : "NULL");
            printf("    Got:      %s '%s'\n", token_name(tokens[i].type),
                   tokens[i].lexeme ? tokens[i].lexeme : "NULL");
            pass = 0;
            goto done;
        }
    }

    if (errors.size != expected_errors) {
        printf("[FAIL] %s\n", name);
        printf("  Expected %zu errors, got %zu\n", expected_errors, errors.size);
        pass = 0;
        goto done;
    }

done:
    if (pass) {
        printf("[PASS] %s\n", name);
    }

    free_tokens(tokens, count);
    return pass;
}

/* ---------- actual tests ---------- */

void run_lexer_tests(void) {
    int passed = 0;
    int total = 0;

    {
        const ExpectedToken exp[] = {
            {TOKEN_IF, "if"},
            {TOKEN_ELSE, "else"},
            {TOKEN_RETURN, "return"},
            {TOKEN_EOF, NULL},
        };
        total++;
        passed += run_test("keywords", "if else return", exp, 4, 0);
    }

    {
        const ExpectedToken exp[] = {
            {TOKEN_INT_LITERAL, "123"},
            {TOKEN_FLOAT_LITERAL, "4.5"},
            {TOKEN_EOF, NULL},
        };
        total++;
        passed += run_test("numbers", "123 4.5", exp, 3, 0);
    }

    {
        const ExpectedToken exp[] = {
            {TOKEN_STRING_LITERAL, "hello"},
            {TOKEN_EOF, NULL},
        };
        total++;
        passed += run_test("string literal", "\"hello\"", exp, 2, 0);
    }

    {
        const ExpectedToken exp[] = {
            {TOKEN_ERROR, NULL},
            {TOKEN_EOF, NULL},
        };
        total++;
        passed += run_test("unterminated string", "\"hello", exp, 2, 1);
    }

    {
        const ExpectedToken exp[] = {
            {TOKEN_PLUS, "+"},
            {TOKEN_PLUS_PLUS, "++"},
            {TOKEN_PLUS_ASSIGN, "+="},
            {TOKEN_EOF, NULL},
        };
        total++;
        passed += run_test("operators longest match", "+ ++ +=", exp, 4, 0);
    }

    printf("\nLexer tests: %d / %d passed\n", passed, total);

    if (passed != total) {
        exit(1);
    }
}
