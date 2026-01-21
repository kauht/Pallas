#include <stdio.h>

#include "./tests/lexer_test.h"
#include "./tests/parser_test.h"
#include "diagnostic.h"

int main() {
    run_lexer_tests();
    run_parser_tests();

    run_parser_structural_tests();

    return 0;
}
