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

        /* Keywords */
        case TOKEN_IMPORT:
            return "TOKEN_IMPORT";
        case TOKEN_IF:
            return "TOKEN_IF";
        case TOKEN_ELSE:
            return "TOKEN_ELSE";
        case TOKEN_FOR:
            return "TOKEN_FOR";
        case TOKEN_WHILE:
            return "TOKEN_WHILE";
        case TOKEN_BREAK:
            return "TOKEN_BREAK";
        case TOKEN_CONTINUE:
            return "TOKEN_CONTINUE";
        case TOKEN_RETURN:
            return "TOKEN_RETURN";
        case TOKEN_STRUCT:
            return "TOKEN_STRUCT";
        case TOKEN_CLASS:
            return "TOKEN_CLASS";
        case TOKEN_TRUE:
            return "TOKEN_TRUE";
        case TOKEN_FALSE:
            return "TOKEN_FALSE";
        case TOKEN_NULL:
            return "TOKEN_NULL";
        case TOKEN_CONST:
            return "TOKEN_CONST";

        /* Types */
        case TOKEN_INT:
            return "TOKEN_INT";
        case TOKEN_FLOAT:
            return "TOKEN_FLOAT";
        case TOKEN_CHAR:
            return "TOKEN_CHAR";
        case TOKEN_STRING:
            return "TOKEN_STRING";
        case TOKEN_I32:
            return "TOKEN_I32";
        case TOKEN_I64:
            return "TOKEN_I64";
        case TOKEN_U32:
            return "TOKEN_U32";
        case TOKEN_U64:
            return "TOKEN_U64";
        case TOKEN_F32:
            return "TOKEN_F32";
        case TOKEN_F64:
            return "TOKEN_F64";

        /* Operators */
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
        case TOKEN_MINUS_ASSIGN:
            return "TOKEN_MINUS_ASSIGN";
        case TOKEN_ARROW:
            return "TOKEN_ARROW";
        case TOKEN_STAR:
            return "TOKEN_STAR";
        case TOKEN_STAR_ASSIGN:
            return "TOKEN_STAR_ASSIGN";
        case TOKEN_SLASH:
            return "TOKEN_SLASH";
        case TOKEN_SLASH_ASSIGN:
            return "TOKEN_SLASH_ASSIGN";
        case TOKEN_PERCENT:
            return "TOKEN_PERCENT";
        case TOKEN_ASSIGN:
            return "TOKEN_ASSIGN";
        case TOKEN_EQUAL:
            return "TOKEN_EQUAL";
        case TOKEN_NOT_EQUAL:
            return "TOKEN_NOT_EQUAL";
        case TOKEN_LESS:
            return "TOKEN_LESS";
        case TOKEN_LESS_EQUAL:
            return "TOKEN_LESS_EQUAL";
        case TOKEN_GREATER:
            return "TOKEN_GREATER";
        case TOKEN_GREATER_EQUAL:
            return "TOKEN_GREATER_EQUAL";
        case TOKEN_LEFT_SHIFT:
            return "TOKEN_LEFT_SHIFT";
        case TOKEN_LSHIFT_EQUALS:
            return "TOKEN_LSHIFT_EQUALS";
        case TOKEN_RIGHT_SHIFT:
            return "TOKEN_RIGHT_SHIFT";
        case TOKEN_RSHIFT_EQUALS:
            return "TOKEN_RSHIFT_EQUALS";
        case TOKEN_CARET:
            return "TOKEN_CARET";
        case TOKEN_XOR_EQUALS:
            return "TOKEN_XOR_EQUALS";
        case TOKEN_AMPERSAND:
            return "TOKEN_AMPERSAND";
        case TOKEN_AND_EQUALS:
            return "TOKEN_AND_EQUALS";
        case TOKEN_LOGICAL_AND:
            return "TOKEN_LOGICAL_AND";
        case TOKEN_PIPE:
            return "TOKEN_PIPE";
        case TOKEN_OR_EQUALS:
            return "TOKEN_OR_EQUALS";
        case TOKEN_LOGICAL_OR:
            return "TOKEN_LOGICAL_OR";
        case TOKEN_LOGICAL_NOT:
            return "TOKEN_LOGICAL_NOT";
        case TOKEN_TILDE:
            return "TOKEN_TILDE";
        case TOKEN_DOT:
            return "TOKEN_DOT";
        case TOKEN_ELLIPSIS:
            return "TOKEN_ELLIPSIS";
        case TOKEN_LPAREN:
            return "TOKEN_LPAREN";
        case TOKEN_RPAREN:
            return "TOKEN_RPAREN";
        case TOKEN_LBRACE:
            return "TOKEN_LBRACE";
        case TOKEN_RBRACE:
            return "TOKEN_RBRACE";
        case TOKEN_LBRACKET:
            return "TOKEN_LBRACKET";
        case TOKEN_RBRACKET:
            return "TOKEN_RBRACKET";
        case TOKEN_SEMICOLON:
            return "TOKEN_SEMICOLON";
        case TOKEN_COMMA:
            return "TOKEN_COMMA";
        case TOKEN_COLON:
            return "TOKEN_COLON";
        case TOKEN_QUESTION:
            return "TOKEN_QUESTION";
        case TOKEN_AT:
            return "TOKEN_AT";
        default:
            return "TOKEN_<unknown>";
    }
}

static int token_eq(Token* t, TokenType type, const char* lexeme) {
    if (t->type != type)
        return 0;
    if (!lexeme)
        return t->lexeme == NULL;
    if (!t->lexeme)
        return 0;
    return strcmp(t->lexeme, lexeme) == 0;
}

static Token* lex_all(const char* src, size_t* out_count, ErrorList* el) {
    Lexer* lx = init_lexer(src, el);
    Token* tokens = run_lexer(lx, out_count);
    free_lexer(lx);
    return tokens;
}

static void free_tokens(Token* toks, size_t n) {
    for (size_t i = 0; i < n; i++)
        free_token(&toks[i]);
    free(toks);
}

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
        printf("[FAIL] %s\n  Expected %zu tokens, got %zu\n", name, expected_count, count);
        pass = 0;
        goto done;
    }

    for (size_t i = 0; i < count; i++) {
        if (!token_eq(&tokens[i], expected[i].type, expected[i].lexeme)) {
            printf("[FAIL] %s\n  Token %zu:\n    Expected: %s '%s'\n    Got:      %s '%s'\n", name,
                   i, token_name(expected[i].type),
                   expected[i].lexeme ? expected[i].lexeme : "NULL", token_name(tokens[i].type),
                   tokens[i].lexeme ? tokens[i].lexeme : "NULL");
            pass = 0;
            goto done;
        }
    }

    if (errors.size != expected_errors) {
        printf("[FAIL] %s\n  Expected %zu errors, got %zu\n", name, expected_errors, errors.size);
        pass = 0;
    }

done:
    if (pass)
        printf("[PASS] %s\n", name);
    free_tokens(tokens, count);
    return pass;
}

/* ---------- individual test cases ---------- */

void run_lexer_tests(void) {
    int passed = 0, total = 0;

    /* --------------- Test 1: Keywords --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IMPORT, "import"},     {TOKEN_IF, "if"},         {TOKEN_ELSE, "else"},
            {TOKEN_FOR, "for"},           {TOKEN_WHILE, "while"},   {TOKEN_BREAK, "break"},
            {TOKEN_CONTINUE, "continue"}, {TOKEN_RETURN, "return"}, {TOKEN_STRUCT, "struct"},
            {TOKEN_CLASS, "class"},       {TOKEN_TRUE, "true"},     {TOKEN_FALSE, "false"},
            {TOKEN_NULL, "null"},         {TOKEN_CONST, "const"},   {TOKEN_EOF, NULL},
        };
        const char* src =
            "import if else for while break continue return struct class true false null const";
        total++;
        passed += run_test("Keywords", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 2: Types --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_INT, "int"},       {TOKEN_FLOAT, "float"}, {TOKEN_CHAR, "char"},
            {TOKEN_STRING, "string"}, {TOKEN_I32, "i32"},     {TOKEN_I64, "i64"},
            {TOKEN_U32, "u32"},       {TOKEN_U64, "u64"},     {TOKEN_F32, "f32"},
            {TOKEN_F64, "f64"},       {TOKEN_EOF, NULL},
        };
        const char* src = "int float char string i32 i64 u32 u64 f32 f64";
        total++;
        passed += run_test("Types", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 3: Identifiers --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "myVar"},      {TOKEN_IDENT, "_hidden123"}, {TOKEN_IDENT, "camelCase"},
            {TOKEN_IDENT, "UPPER_CASE"}, {TOKEN_IDENT, "x"},          {TOKEN_EOF, NULL},
        };
        const char* src = "myVar _hidden123 camelCase UPPER_CASE x";
        total++;
        passed += run_test("Identifiers", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 4: Numbers --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_INT_LITERAL, "0"},         {TOKEN_INT_LITERAL, "123"},
            {TOKEN_INT_LITERAL, "9999"},      {TOKEN_FLOAT_LITERAL, "4.56"},
            {TOKEN_FLOAT_LITERAL, "7.89"},    {TOKEN_FLOAT_LITERAL, "0.5"},
            {TOKEN_FLOAT_LITERAL, "123.456"}, {TOKEN_EOF, NULL},
        };
        const char* src = "0 123 9999 4.56 7.89 0.5 123.456";
        total++;
        passed += run_test("Numbers", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 5: Character Literals --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_CHAR_LITERAL, "a"},  {TOKEN_CHAR_LITERAL, "\\"}, {TOKEN_CHAR_LITERAL, "z"},
            {TOKEN_CHAR_LITERAL, "\n"}, {TOKEN_CHAR_LITERAL, "\""}, {TOKEN_EOF, NULL},
        };
        const char* src = "'a' '\\\\' 'z' '\\n' '\\\"'";
        total++;
        passed += run_test("Character Literals", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 6: String Literals --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_STRING_LITERAL, ""},
            {TOKEN_STRING_LITERAL, "hello"},
            {TOKEN_STRING_LITERAL, "line\nbreak"},
            {TOKEN_STRING_LITERAL, "escaped\"quote"},
            {TOKEN_STRING_LITERAL, "tab\ttab"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "\"\" \"hello\" \"line\\nbreak\" \"escaped\\\"quote\" \"tab\\ttab\"";
        total++;
        passed += run_test("String Literals", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 7: Operators --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_PLUS, "+"},
            {TOKEN_PLUS_PLUS, "++"},
            {TOKEN_PLUS_ASSIGN, "+="},
            {TOKEN_MINUS, "-"},
            {TOKEN_MINUS_MINUS, "--"},
            {TOKEN_MINUS_ASSIGN, "-="},
            {TOKEN_ARROW, "->"},
            {TOKEN_STAR, "*"},
            {TOKEN_STAR_ASSIGN, "*="},
            {TOKEN_SLASH, "/"},
            {TOKEN_SLASH_ASSIGN, "/="},
            {TOKEN_PERCENT, "%"},
            {TOKEN_ASSIGN, "="},
            {TOKEN_EQUAL, "=="},
            {TOKEN_NOT_EQUAL, "!="},
            {TOKEN_LESS, "<"},
            {TOKEN_LESS_EQUAL, "<="},
            {TOKEN_GREATER, ">"},
            {TOKEN_GREATER_EQUAL, ">="},
            {TOKEN_LEFT_SHIFT, "<<"},
            {TOKEN_LSHIFT_EQUALS, "<<="},
            {TOKEN_RIGHT_SHIFT, ">>"},
            {TOKEN_RSHIFT_EQUALS, ">>="},
            {TOKEN_CARET, "^"},
            {TOKEN_XOR_EQUALS, "^="},
            {TOKEN_AMPERSAND, "&"},
            {TOKEN_AND_EQUALS, "&="},
            {TOKEN_LOGICAL_AND, "&&"},
            {TOKEN_PIPE, "|"},
            {TOKEN_OR_EQUALS, "|="},
            {TOKEN_LOGICAL_OR, "||"},
            {TOKEN_LOGICAL_NOT, "!"},
            {TOKEN_TILDE, "~"},
            {TOKEN_DOT, "."},
            {TOKEN_ELLIPSIS, "..."},
            {TOKEN_EOF, NULL},
        };
        const char* src =
            "+ ++ += - -- -= -> * *= / /= % = == != < <= > >= << <<= >> >>= ^ ^= & &= && | |= || ! "
            "~ . ...";
        total++;
        passed += run_test("Operators", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 8: Punctuation --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_LPAREN, "("},    {TOKEN_RPAREN, ")"},   {TOKEN_LBRACE, "{"},
            {TOKEN_RBRACE, "}"},    {TOKEN_LBRACKET, "["}, {TOKEN_RBRACKET, "]"},
            {TOKEN_SEMICOLON, ";"}, {TOKEN_COMMA, ","},    {TOKEN_COLON, ":"},
            {TOKEN_QUESTION, "?"},  {TOKEN_AT, "@"},       {TOKEN_EOF, NULL},
        };
        const char* src = "( ) { } [ ] ; , : ? @";
        total++;
        passed += run_test("Punctuation", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 9: Comments --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_INT_LITERAL, "1"},
            {TOKEN_PLUS, "+"},
            {TOKEN_INT_LITERAL, "2"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "1 // This is a comment\n+ 2 /* multi\nline\ncomment */";
        total++;
        passed += run_test("Comments", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 10: Mixed Everything --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IF, "if"},         {TOKEN_LPAREN, "("},
            {TOKEN_IDENT, "x"},       {TOKEN_EQUAL, "=="},
            {TOKEN_INT_LITERAL, "5"}, {TOKEN_RPAREN, ")"},
            {TOKEN_LBRACE, "{"},      {TOKEN_IDENT, "println"},
            {TOKEN_LPAREN, "("},      {TOKEN_STRING_LITERAL, "x is 5"},
            {TOKEN_RPAREN, ")"},      {TOKEN_SEMICOLON, ";"},
            {TOKEN_RBRACE, "}"},      {TOKEN_EOF, NULL},
        };
        const char* src = "if (x == 5) { println(\"x is 5\"); }";
        total++;
        passed += run_test("Mixed Example", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 11: Type Declarations --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "x"},     {TOKEN_COLON, ":"},       {TOKEN_I32, "i32"},
            {TOKEN_ASSIGN, "="},    {TOKEN_INT_LITERAL, "5"}, {TOKEN_SEMICOLON, ";"},
            {TOKEN_IDENT, "ptr"},   {TOKEN_COLON, ":"},       {TOKEN_I32, "i32"},
            {TOKEN_STAR, "*"},      {TOKEN_ASSIGN, "="},      {TOKEN_NULL, "null"},
            {TOKEN_SEMICOLON, ";"}, {TOKEN_EOF, NULL},
        };
        const char* src = "x: i32 = 5; ptr: i32* = null;";
        total++;
        passed += run_test("Type Declarations", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    printf("\nLexer tests: %d / %d passed\n", passed, total);
    if (passed != total) {
        printf("Some tests failed. Fix your lexer!\n");
        exit(1);
    }
}
