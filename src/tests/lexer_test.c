#include "lexer_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- utilities ---------- */

/* Forward declaration */
static const char* token_name(TokenType t);

/* Print a single token in a nice format */
static void print_token(const Token* t, size_t index) {
    printf("  [%3zu] %-25s", index, token_name(t->type));
    if (t->lexeme) {
        printf(" '%s'", t->lexeme);
    }
    printf(" (line %zu, col %zu)\n", t->line, t->column);
}

/* Print all tokens in a pretty table */
static void print_token_stream(const Token* tokens, size_t count) {
    printf("\n");
    printf("+========================================================================+\n");
    printf("|                          TOKEN STREAM                                  |\n");
    printf("+========================================================================+\n");
    for (size_t i = 0; i < count; i++) {
        printf("| ");
        print_token(&tokens[i], i);
    }
    printf("+========================================================================+\n");
    printf("\n");
}

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
        case TOKEN_INCLUDE:
            return "TOKEN_INCLUDE";
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
        case TOKEN_PUBLIC:
            return "TOKEN_PUBLIC";
        case TOKEN_PRIVATE:
            return "TOKEN_PRIVATE";
        case TOKEN_NEW:
            return "TOKEN_NEW";
        case TOKEN_DELETE:
            return "TOKEN_DELETE";
        case TOKEN_TRUE:
            return "TOKEN_TRUE";
        case TOKEN_FALSE:
            return "TOKEN_FALSE";
        case TOKEN_NULL:
            return "TOKEN_NULL";
        case TOKEN_CONST:
            return "TOKEN_CONST";
        case TOKEN_VOID:
            return "TOKEN_VOID";
        case TOKEN_MATCH:
            return "TOKEN_MATCH";
        case TOKEN_ENUM:
            return "TOKEN_ENUM";

        /* Types */
        case TOKEN_INT:
            return "TOKEN_INT";
        case TOKEN_FLOAT:
            return "TOKEN_FLOAT";
        case TOKEN_DOUBLE:
            return "TOKEN_DOUBLE";
        case TOKEN_CHAR:
            return "TOKEN_CHAR";
        case TOKEN_STRING:
            return "TOKEN_STRING";
        case TOKEN_BOOL:
            return "TOKEN_BOOL";
        case TOKEN_I8:
            return "TOKEN_I8";
        case TOKEN_I16:
            return "TOKEN_I16";
        case TOKEN_I32:
            return "TOKEN_I32";
        case TOKEN_I64:
            return "TOKEN_I64";
        case TOKEN_U8:
            return "TOKEN_U8";
        case TOKEN_U16:
            return "TOKEN_U16";
        case TOKEN_U32:
            return "TOKEN_U32";
        case TOKEN_U64:
            return "TOKEN_U64";
        case TOKEN_F8:
            return "TOKEN_F8";
        case TOKEN_F16:
            return "TOKEN_F16";
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
        case TOKEN_DOUBLE_COLON:
            return "TOKEN_DOUBLE_COLON";
        case TOKEN_FAT_ARROW:
            return "TOKEN_FAT_ARROW";
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

/* Lex source string for testing */
static Token* lex_all(const char* src, size_t* out_count, ErrorList* el) {
    Lexer* lx = init_lexer("C:\\Users\\sam\\Desktop\\pallas\\examples\\pallas.pal", src, el);
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
        printf("  \033[31;1m[FAIL]\033[0m %s\n", name);
        printf("    Expected %zu tokens, got %zu\n", expected_count, count);
        print_token_stream(tokens, count);
        pass = 0;
        goto done;
    }

    for (size_t i = 0; i < count; i++) {
        if (!token_eq(&tokens[i], expected[i].type, expected[i].lexeme)) {
            printf("  \033[31;1m[FAIL]\033[0m %s\n", name);
            printf("    Token %zu mismatch:\n", i);
            printf("      Expected: %s '%s'\n",
                   token_name(expected[i].type), 
                   expected[i].lexeme ? expected[i].lexeme : "(null)");
            printf("      Got:      %s '%s'\n",
                   token_name(tokens[i].type), 
                   tokens[i].lexeme ? tokens[i].lexeme : "(null)");
            print_token_stream(tokens, count);
            pass = 0;
            goto done;
        }
    }

    if (errors.size != expected_errors) {
        printf("  \033[31;1m[FAIL]\033[0m %s\n", name);
        printf("    Expected %zu diagnostics, got %zu\n", expected_errors, errors.size);
        if (errors.size > 0) {
            printf("    Diagnostics:\n");
            for (size_t i = 0; i < errors.size; i++) {
                char* formatted = errors_format(&errors.items[i]);
                if (formatted) {
                    printf("      %s\n", formatted);
                    free(formatted);
                }
            }
        }
        pass = 0;
    }

done:
    if (pass)
        printf("  \033[32;1m[PASS]\033[0m %s\n", name);
    free_tokens(tokens, count);
    errors_clear(&errors);
    return pass;
}

/* ---------- individual test cases ---------- */

/* Lex and print the example file */
static void lex_example_file(void) {
    const char* example_path = "C:\\Users\\sam\\Desktop\\pallas\\examples\\pallas.pal";
    
    printf("\n");
    printf("+========================================================================+\n");
    printf("|                      LEXING EXAMPLE FILE                               |\n");
    printf("+------------------------------------------------------------------------+\n");
    printf("| File: %-65s|\n", example_path);
    printf("+========================================================================+\n");
    
    FILE* file = NULL;
#ifdef _MSC_VER
    errno_t err = fopen_s(&file, example_path, "rb");
    if (err != 0 || !file) {
#else
    file = fopen(example_path, "rb");
    if (!file) {
#endif
        printf("⚠ Could not open example file: %s\n", example_path);
        printf("  Skipping example file tokenization.\n\n");
        return;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* source = (char*)malloc(size + 1);
    if (!source) {
        printf("⚠ Failed to allocate memory for file content\n\n");
        fclose(file);
        return;
    }
    
    fread(source, 1, size, file);
    source[size] = '\0';
    fclose(file);
    
    ErrorList errors = {0};
    size_t count = 0;
    Token* tokens = lex_all(source, &count, &errors);
    
    print_token_stream(tokens, count);
    
    if (errors.size > 0) {
        printf("+========================================================================+\n");
        printf("|                          DIAGNOSTICS                                   |\n");
        printf("+------------------------------------------------------------------------+\n");
        for (size_t i = 0; i < errors.size; i++) {
            char* formatted = errors_format(&errors.items[i]);
            if (formatted) {
                printf("| %s\n", formatted);
                free(formatted);
            }
        }
        printf("+========================================================================+\n");
        printf("\n");
    }
    
    printf("Total tokens: %zu\n", count);
    printf("Total diagnostics: %zu\n\n", errors.size);
    
    free_tokens(tokens, count);
    errors_clear(&errors);
    free(source);
}

void run_lexer_tests(void) {
    int passed = 0, total = 0;
    
    printf("\n");
    printf("+========================================================================+\n");
    printf("|                          LEXER TEST SUITE                              |\n");
    printf("+========================================================================+\n");
    printf("\n");

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
            {TOKEN_INT, "int"},       {TOKEN_FLOAT, "float"},   {TOKEN_DOUBLE, "double"},
            {TOKEN_CHAR, "char"},     {TOKEN_STRING, "string"}, {TOKEN_BOOL, "bool"},
            {TOKEN_I32, "i32"},       {TOKEN_I64, "i64"},       {TOKEN_U32, "u32"},
            {TOKEN_U64, "u64"},       {TOKEN_F32, "f32"},       {TOKEN_F64, "f64"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "int float double char string bool i32 i64 u32 u64 f32 f64";
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

    /* --------------- Test 12: Variadic Functions --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "printf"},  {TOKEN_LPAREN, "("},     {TOKEN_IDENT, "fmt"},
            {TOKEN_COLON, ":"},       {TOKEN_IDENT, "str"},    {TOKEN_COMMA, ","},
            {TOKEN_ELLIPSIS, "..."},  {TOKEN_RPAREN, ")"},     {TOKEN_LBRACE, "{"},
            {TOKEN_RBRACE, "}"},      {TOKEN_EOF, NULL},
        };
        const char* src = "printf(fmt: str, ...) {}";
        total++;
        passed += run_test("Variadic Functions", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 13: Struct Declaration --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_STRUCT, "struct"}, {TOKEN_IDENT, "Point"},  {TOKEN_LBRACE, "{"},
            {TOKEN_IDENT, "x"},       {TOKEN_COLON, ":"},      {TOKEN_I32, "i32"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_IDENT, "y"},      {TOKEN_COLON, ":"},
            {TOKEN_I32, "i32"},       {TOKEN_SEMICOLON, ";"},  {TOKEN_RBRACE, "}"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "struct Point { x: i32; y: i32; }";
        total++;
        passed += run_test("Struct Declaration", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 14: Class Declaration --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_CLASS, "class"},   {TOKEN_IDENT, "Vec3"},   {TOKEN_LBRACE, "{"},
            {TOKEN_IDENT, "x"},       {TOKEN_COLON, ":"},      {TOKEN_F32, "f32"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_RBRACE, "}"},     {TOKEN_EOF, NULL},
        };
        const char* src = "class Vec3 { x: f32; }";
        total++;
        passed += run_test("Class Declaration", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 15: Pointer Types --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "ptr"},     {TOKEN_COLON, ":"},      {TOKEN_I32, "i32"},
            {TOKEN_STAR, "*"},        {TOKEN_STAR, "*"},       {TOKEN_SEMICOLON, ";"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "ptr: i32**;";
        total++;
        passed += run_test("Pointer Types", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 16: If-Else Statement --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IF, "if"},         {TOKEN_LPAREN, "("},     {TOKEN_IDENT, "x"},
            {TOKEN_GREATER, ">"},     {TOKEN_INT_LITERAL, "0"}, {TOKEN_RPAREN, ")"},
            {TOKEN_LBRACE, "{"},      {TOKEN_RBRACE, "}"},     {TOKEN_ELSE, "else"},
            {TOKEN_LBRACE, "{"},      {TOKEN_RBRACE, "}"},     {TOKEN_EOF, NULL},
        };
        const char* src = "if (x > 0) {} else {}";
        total++;
        passed += run_test("If-Else Statement", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 17: While Loop --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_WHILE, "while"},   {TOKEN_LPAREN, "("},     {TOKEN_TRUE, "true"},
            {TOKEN_RPAREN, ")"},      {TOKEN_LBRACE, "{"},     {TOKEN_BREAK, "break"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_RBRACE, "}"},     {TOKEN_EOF, NULL},
        };
        const char* src = "while (true) { break; }";
        total++;
        passed += run_test("While Loop", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 18: For Loop --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_FOR, "for"},       {TOKEN_LPAREN, "("},     {TOKEN_IDENT, "i"},
            {TOKEN_COLON, ":"},       {TOKEN_I32, "i32"},      {TOKEN_ASSIGN, "="},
            {TOKEN_INT_LITERAL, "0"}, {TOKEN_SEMICOLON, ";"},  {TOKEN_IDENT, "i"},
            {TOKEN_LESS, "<"},        {TOKEN_INT_LITERAL, "10"}, {TOKEN_SEMICOLON, ";"},
            {TOKEN_IDENT, "i"},       {TOKEN_PLUS_PLUS, "++"}, {TOKEN_RPAREN, ")"},
            {TOKEN_LBRACE, "{"},      {TOKEN_RBRACE, "}"},     {TOKEN_EOF, NULL},
        };
        const char* src = "for (i: i32 = 0; i < 10; i++) {}";
        total++;
        passed += run_test("For Loop", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 19: Return Statement --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_RETURN, "return"}, {TOKEN_INT_LITERAL, "42"}, {TOKEN_SEMICOLON, ";"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "return 42;";
        total++;
        passed += run_test("Return Statement", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 20: Import Statement --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IMPORT, "import"}, {TOKEN_IDENT, "std"},    {TOKEN_DOT, "."},
            {TOKEN_IDENT, "io"},      {TOKEN_SEMICOLON, ";"},  {TOKEN_EOF, NULL},
        };
        const char* src = "import std.io;";
        total++;
        passed += run_test("Import Statement", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 21: Function with Return Type --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "add"},     {TOKEN_LPAREN, "("},     {TOKEN_IDENT, "a"},
            {TOKEN_COLON, ":"},       {TOKEN_I32, "i32"},      {TOKEN_COMMA, ","},
            {TOKEN_IDENT, "b"},       {TOKEN_COLON, ":"},      {TOKEN_I32, "i32"},
            {TOKEN_RPAREN, ")"},      {TOKEN_COLON, ":"},      {TOKEN_I32, "i32"},
            {TOKEN_LBRACE, "{"},      {TOKEN_RETURN, "return"}, {TOKEN_IDENT, "a"},
            {TOKEN_PLUS, "+"},        {TOKEN_IDENT, "b"},      {TOKEN_SEMICOLON, ";"},
            {TOKEN_RBRACE, "}"},      {TOKEN_EOF, NULL},
        };
        const char* src = "add(a: i32, b: i32): i32 { return a + b; }";
        total++;
        passed += run_test("Function with Return Type", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 22: Const Variables --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_CONST, "const"},   {TOKEN_IDENT, "PI"},     {TOKEN_COLON, ":"},
            {TOKEN_F32, "f32"},       {TOKEN_ASSIGN, "="},     {TOKEN_FLOAT_LITERAL, "3.14"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_EOF, NULL},
        };
        const char* src = "const PI: f32 = 3.14;";
        total++;
        passed += run_test("Const Variables", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 23: Boolean Literals --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "flag"},    {TOKEN_COLON, ":"},      {TOKEN_BOOL, "bool"},
            {TOKEN_ASSIGN, "="},      {TOKEN_TRUE, "true"},    {TOKEN_SEMICOLON, ";"},
            {TOKEN_IDENT, "other"},   {TOKEN_COLON, ":"},      {TOKEN_BOOL, "bool"},
            {TOKEN_ASSIGN, "="},      {TOKEN_FALSE, "false"},  {TOKEN_SEMICOLON, ";"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "flag: bool = true; other: bool = false;";
        total++;
        passed += run_test("Boolean Literals", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 24: Logical Operators --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "result"},  {TOKEN_ASSIGN, "="},     {TOKEN_IDENT, "a"},
            {TOKEN_LOGICAL_AND, "&&"}, {TOKEN_IDENT, "b"},     {TOKEN_LOGICAL_OR, "||"},
            {TOKEN_IDENT, "c"},       {TOKEN_SEMICOLON, ";"},  {TOKEN_EOF, NULL},
        };
        const char* src = "result = a && b || c;";
        total++;
        passed += run_test("Logical Operators", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 25: Bitwise Operators --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "x"},       {TOKEN_ASSIGN, "="},     {TOKEN_IDENT, "a"},
            {TOKEN_AMPERSAND, "&"},   {TOKEN_IDENT, "b"},      {TOKEN_PIPE, "|"},
            {TOKEN_IDENT, "c"},       {TOKEN_CARET, "^"},      {TOKEN_IDENT, "d"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_EOF, NULL},
        };
        const char* src = "x = a & b | c ^ d;";
        total++;
        passed += run_test("Bitwise Operators", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 26: Shift Operators --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "x"},       {TOKEN_ASSIGN, "="},     {TOKEN_IDENT, "a"},
            {TOKEN_LEFT_SHIFT, "<<"}, {TOKEN_INT_LITERAL, "5"}, {TOKEN_SEMICOLON, ";"},
            {TOKEN_IDENT, "y"},       {TOKEN_ASSIGN, "="},     {TOKEN_IDENT, "b"},
            {TOKEN_RIGHT_SHIFT, ">>"}, {TOKEN_INT_LITERAL, "3"}, {TOKEN_SEMICOLON, ";"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "x = a << 5; y = b >> 3;";
        total++;
        passed += run_test("Shift Operators", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 27: Continue Statement --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_WHILE, "while"},   {TOKEN_LPAREN, "("},     {TOKEN_TRUE, "true"},
            {TOKEN_RPAREN, ")"},      {TOKEN_LBRACE, "{"},     {TOKEN_CONTINUE, "continue"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_RBRACE, "}"},     {TOKEN_EOF, NULL},
        };
        const char* src = "while (true) { continue; }";
        total++;
        passed += run_test("Continue Statement", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 28: Null Literal --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "ptr"},     {TOKEN_COLON, ":"},      {TOKEN_I32, "i32"},
            {TOKEN_STAR, "*"},        {TOKEN_ASSIGN, "="},     {TOKEN_NULL, "null"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_EOF, NULL},
        };
        const char* src = "ptr: i32* = null;";
        total++;
        passed += run_test("Null Literal", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 29: All Sized Integer Types --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "a"},       {TOKEN_COLON, ":"},      {TOKEN_I8, "i8"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_IDENT, "b"},      {TOKEN_COLON, ":"},
            {TOKEN_I16, "i16"},       {TOKEN_SEMICOLON, ";"},  {TOKEN_IDENT, "c"},
            {TOKEN_COLON, ":"},       {TOKEN_U8, "u8"},        {TOKEN_SEMICOLON, ";"},
            {TOKEN_IDENT, "d"},       {TOKEN_COLON, ":"},      {TOKEN_U16, "u16"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_EOF, NULL},
        };
        const char* src = "a: i8; b: i16; c: u8; d: u16;";
        total++;
        passed += run_test("All Sized Integer Types", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 30: Arrow Operator --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "ptr"},     {TOKEN_ARROW, "->"},     {TOKEN_IDENT, "field"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_EOF, NULL},
        };
        const char* src = "ptr->field;";
        total++;
        passed += run_test("Arrow Operator", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 31: Public/Private Keywords --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_CLASS, "class"},   {TOKEN_IDENT, "MyClass"}, {TOKEN_LBRACE, "{"},
            {TOKEN_PUBLIC, "public"}, {TOKEN_IDENT, "x"},      {TOKEN_COLON, ":"},
            {TOKEN_I32, "i32"},       {TOKEN_SEMICOLON, ";"},  {TOKEN_PRIVATE, "private"},
            {TOKEN_IDENT, "y"},       {TOKEN_COLON, ":"},      {TOKEN_I32, "i32"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_RBRACE, "}"},     {TOKEN_EOF, NULL},
        };
        const char* src = "class MyClass { public x: i32; private y: i32; }";
        total++;
        passed += run_test("Public/Private Keywords", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 32: Constructor --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "MyClass"}, {TOKEN_LPAREN, "("},     {TOKEN_RPAREN, ")"},
            {TOKEN_LBRACE, "{"},      {TOKEN_RBRACE, "}"},     {TOKEN_EOF, NULL},
        };
        const char* src = "MyClass() {}";
        total++;
        passed += run_test("Constructor", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 33: Destructor --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_TILDE, "~"},       {TOKEN_IDENT, "MyClass"}, {TOKEN_LPAREN, "("},
            {TOKEN_RPAREN, ")"},      {TOKEN_LBRACE, "{"},     {TOKEN_RBRACE, "}"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "~MyClass() {}";
        total++;
        passed += run_test("Destructor", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 34: New/Delete Keywords --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "ptr"},     {TOKEN_COLON, ":"},      {TOKEN_I32, "i32"},
            {TOKEN_STAR, "*"},        {TOKEN_ASSIGN, "="},     {TOKEN_NEW, "new"},
            {TOKEN_LPAREN, "("},      {TOKEN_I32, "i32"},      {TOKEN_RPAREN, ")"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_DELETE, "delete"}, {TOKEN_LPAREN, "("},
            {TOKEN_IDENT, "ptr"},     {TOKEN_RPAREN, ")"},     {TOKEN_SEMICOLON, ";"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "ptr: i32* = new(i32); delete(ptr);";
        total++;
        passed += run_test("New/Delete Keywords", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 35: Void Type --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "func"},    {TOKEN_LPAREN, "("},     {TOKEN_RPAREN, ")"},
            {TOKEN_COLON, ":"},       {TOKEN_VOID, "void"},    {TOKEN_LBRACE, "{"},
            {TOKEN_RBRACE, "}"},      {TOKEN_EOF, NULL},
        };
        const char* src = "func(): void {}";
        total++;
        passed += run_test("Void Type", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 36: Method Call with Dot --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "obj"},     {TOKEN_DOT, "."},        {TOKEN_IDENT, "method"},
            {TOKEN_LPAREN, "("},      {TOKEN_RPAREN, ")"},     {TOKEN_SEMICOLON, ";"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "obj.method();";
        total++;
        passed += run_test("Method Call with Dot", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 37: Compound Assignment Operators --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "x"},       {TOKEN_PLUS_ASSIGN, "+="}, {TOKEN_INT_LITERAL, "5"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_IDENT, "y"},      {TOKEN_MINUS_ASSIGN, "-="},
            {TOKEN_INT_LITERAL, "3"}, {TOKEN_SEMICOLON, ";"},  {TOKEN_IDENT, "z"},
            {TOKEN_STAR_ASSIGN, "*="}, {TOKEN_INT_LITERAL, "2"}, {TOKEN_SEMICOLON, ";"},
            {TOKEN_IDENT, "w"},       {TOKEN_SLASH_ASSIGN, "/="}, {TOKEN_INT_LITERAL, "4"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_EOF, NULL},
        };
        const char* src = "x += 5; y -= 3; z *= 2; w /= 4;";
        total++;
        passed += run_test("Compound Assignment Operators", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 38: Object Instantiation --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "MyClass"}, {TOKEN_IDENT, "obj"},    {TOKEN_LPAREN, "("},
            {TOKEN_RPAREN, ")"},      {TOKEN_SEMICOLON, ";"},  {TOKEN_EOF, NULL},
        };
        const char* src = "MyClass obj();";
        total++;
        passed += run_test("Object Instantiation", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 39: Match Keyword --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_MATCH, "match"},   {TOKEN_LPAREN, "("},     {TOKEN_IDENT, "x"},
            {TOKEN_RPAREN, ")"},      {TOKEN_LBRACE, "{"},     {TOKEN_RBRACE, "}"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "match (x) {}";
        total++;
        passed += run_test("Match Keyword", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 40: Enum Declaration --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_ENUM, "enum"},     {TOKEN_IDENT, "Color"},  {TOKEN_LBRACE, "{"},
            {TOKEN_IDENT, "Red"},     {TOKEN_COMMA, ","},      {TOKEN_IDENT, "Green"},
            {TOKEN_COMMA, ","},       {TOKEN_IDENT, "Blue"},   {TOKEN_RBRACE, "}"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "enum Color { Red, Green, Blue }";
        total++;
        passed += run_test("Enum Declaration", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 41: Double Colon Operator --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "Color"},   {TOKEN_DOUBLE_COLON, "::"}, {TOKEN_IDENT, "Red"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_EOF, NULL},
        };
        const char* src = "Color::Red;";
        total++;
        passed += run_test("Double Colon Operator", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 42: Fat Arrow Operator --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "x"},       {TOKEN_FAT_ARROW, "=>"}, {TOKEN_IDENT, "y"},
            {TOKEN_SEMICOLON, ";"},   {TOKEN_EOF, NULL},
        };
        const char* src = "x => y;";
        total++;
        passed += run_test("Fat Arrow Operator", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 43: Pattern Match with Enum --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_MATCH, "match"},   {TOKEN_LPAREN, "("},     {TOKEN_IDENT, "shape"},
            {TOKEN_RPAREN, ")"},      {TOKEN_LBRACE, "{"},     {TOKEN_IDENT, "Shape"},
            {TOKEN_DOUBLE_COLON, "::"}, {TOKEN_IDENT, "Circle"}, {TOKEN_LPAREN, "("},
            {TOKEN_IDENT, "r"},       {TOKEN_RPAREN, ")"},     {TOKEN_FAT_ARROW, "=>"},
            {TOKEN_LBRACE, "{"},      {TOKEN_RBRACE, "}"},     {TOKEN_RBRACE, "}"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "match (shape) { Shape::Circle(r) => {} }";
        total++;
        passed += run_test("Pattern Match with Enum", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 44: Enum with Data --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_ENUM, "enum"},     {TOKEN_IDENT, "Option"}, {TOKEN_LBRACE, "{"},
            {TOKEN_IDENT, "Some"},    {TOKEN_LPAREN, "("},     {TOKEN_IDENT, "value"},
            {TOKEN_COLON, ":"},       {TOKEN_I32, "i32"},      {TOKEN_RPAREN, ")"},
            {TOKEN_COMMA, ","},       {TOKEN_IDENT, "None"},   {TOKEN_RBRACE, "}"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "enum Option { Some(value: i32), None }";
        total++;
        passed += run_test("Enum with Data", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 45: Wildcard Pattern --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_IDENT, "_"},       {TOKEN_FAT_ARROW, "=>"}, {TOKEN_LBRACE, "{"},
            {TOKEN_RBRACE, "}"},      {TOKEN_EOF, NULL},
        };
        const char* src = "_ => {}";
        total++;
        passed += run_test("Wildcard Pattern", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* --------------- Test 46: Complete Match Statement --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_MATCH, "match"},   {TOKEN_LPAREN, "("},     {TOKEN_IDENT, "color"},
            {TOKEN_RPAREN, ")"},      {TOKEN_LBRACE, "{"},     {TOKEN_IDENT, "Color"},
            {TOKEN_DOUBLE_COLON, "::"}, {TOKEN_IDENT, "Red"},  {TOKEN_PIPE, "|"},
            {TOKEN_IDENT, "Color"},   {TOKEN_DOUBLE_COLON, "::"}, {TOKEN_IDENT, "Blue"},
            {TOKEN_FAT_ARROW, "=>"},  {TOKEN_LBRACE, "{"},     {TOKEN_RBRACE, "}"},
            {TOKEN_RBRACE, "}"},      {TOKEN_EOF, NULL},
        };
        const char* src = "match (color) { Color::Red | Color::Blue => {} }";
        total++;
        passed += run_test("Complete Match Statement", src, exp, sizeof(exp) / sizeof(exp[0]), 0);
    }

    /* Additional error-recovery tests */

    /* --------------- Test 47: Unterminated String Literal --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_ERROR, NULL},
            {TOKEN_EOF, NULL},
        };
        const char* src = "\"hello";
        total++;
        passed += run_test("Unterminated String Literal", src, exp, sizeof(exp) / sizeof(exp[0]), 1);
    }

    /* --------------- Test 48: Unterminated Character Literal --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_ERROR, NULL},
            {TOKEN_EOF, NULL},
        };
        const char* src = "'a";
        total++;
        passed += run_test("Unterminated Character Literal", src, exp, sizeof(exp) / sizeof(exp[0]), 1);
    }

    /* --------------- Test 49: Unterminated Block Comment --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_INT_LITERAL, "1"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "1 /* unclosed";
        total++;
        passed += run_test("Unterminated Block Comment", src, exp, sizeof(exp) / sizeof(exp[0]), 1);
    }

    /* --------------- Test 50: Invalid Number (Too Many Decimals) --------------- */
    {
        const ExpectedToken exp[] = {
            {TOKEN_FLOAT_LITERAL, "1.2"},
            {TOKEN_DOT, "."},
            {TOKEN_INT_LITERAL, "3"},
            {TOKEN_EOF, NULL},
        };
        const char* src = "1.2.3";
        total++;
        passed += run_test("Invalid Number - Too Many Decimals", src, exp, sizeof(exp) / sizeof(exp[0]), 1);
    }

    /* Print results */
    printf("\n");
    printf("+========================================================================+\n");
    printf("|                           TEST RESULTS                                 |\n");
    printf("+------------------------------------------------------------------------+\n");
    if (passed == total) {
        printf("|  \033[32;1m[PASS]\033[0m All tests passed! (%d/%d)                                 |\n", passed, total);
    } else {
        printf("|  \033[31;1m[FAIL]\033[0m Some tests failed: \033[32;1m%d\033[0m/\033[31;1m%d\033[0m passed                       |\n", passed, total);
    }
    printf("+========================================================================+\n");
    printf("\n");
    if (passed != total) {
        exit(1);
    }
    
    /* Now lex and display the example file */
    lex_example_file();
}