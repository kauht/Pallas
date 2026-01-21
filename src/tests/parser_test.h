#ifndef PARSER_TEST_H
#define PARSER_TEST_H

#include <stddef.h>

#include "../lex/lexer.h"
#include "../parse/parser.h"
#include "diagnostic.h"

void run_parser_tests(void);
void run_parser_structural_tests(void);

#endif