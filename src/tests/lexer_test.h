#ifndef LEXER_TEST_H
#define LEXER_TEST_H

#include <stddef.h>

#include "../lex/lexer.h"
#include "errors.h"

void run_lexer_tests(void);
void assert_token(Token t, TokenType expected_type, const char* expected_lexeme);
Token* lex_string(const char* src, size_t* out_count, ErrorList* el);

#endif
