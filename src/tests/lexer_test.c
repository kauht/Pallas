/* AI GEMINATED */

#include "lexer_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Assert token matches expected type + lexeme */
void assert_token(Token t, TokenType expected_type, const char* expected_lexeme) {
    if (t.type != expected_type) {
        printf("FAIL: Expected type %d, got %d at line %zu\n", expected_type, t.type, t.line);
        exit(1);
    }
    if (expected_lexeme && (!t.lexeme || strcmp(t.lexeme, expected_lexeme) != 0)) {
        printf("FAIL: Expected lexeme '%s', got '%s' at line %zu\n", expected_lexeme,
               t.lexeme ? t.lexeme : "NULL", t.line);
        exit(1);
    }
}

/* Run lexer on a string and return tokens + count */
Token* lex_string(const char* src, size_t* out_count, ErrorList* el) {
    Lexer* lx = init_lexer(src, el);
    size_t capacity = 16, count = 0;
    Token* tokens = malloc(sizeof(Token) * capacity);

    while (1) {
        if (count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, sizeof(Token) * capacity);
        }
        Token t = next_token(lx);
        tokens[count++] = t;
        if (t.type == TOKEN_EOF)
            break;
    }

    free_lexer(lx);
    *out_count = count;
    return tokens;
}

/* Example test cases */
void run_lexer_tests(void) {
    ErrorList errors = {0};  // make sure you have an ErrorList structure

    /* Test keywords */
    size_t count;
    Token* tokens = lex_string("if else return 42", &count, &errors);
    assert_token(tokens[0], TOKEN_IF, "if");
    assert_token(tokens[1], TOKEN_ELSE, "else");
    assert_token(tokens[2], TOKEN_RETURN, "return");
    assert_token(tokens[3], TOKEN_INT_LITERAL, "42");
    assert_token(tokens[4], TOKEN_EOF, NULL);
    for (size_t i = 0; i < count; i++)
        free_token(&tokens[i]);
    free(tokens);

    /* Test operators */
    tokens = lex_string("+ - ++ -- && || =", &count, &errors);
    assert_token(tokens[0], TOKEN_PLUS, "+");
    assert_token(tokens[1], TOKEN_MINUS, "-");
    assert_token(tokens[2], TOKEN_PLUS_PLUS, "++");
    assert_token(tokens[3], TOKEN_MINUS_MINUS, "--");
    assert_token(tokens[4], TOKEN_LOGICAL_AND, "&&");
    assert_token(tokens[5], TOKEN_LOGICAL_OR, "||");
    assert_token(tokens[6], TOKEN_ASSIGN, "=");
    assert_token(tokens[7], TOKEN_EOF, NULL);
    for (size_t i = 0; i < count; i++)
        free_token(&tokens[i]);
    free(tokens);

    printf("All lexer tests passed!\n");
}
