#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <string>
#include "frontend/scanner.h"

using namespace pallas::frontend;

TEST_CASE("keywords are recognized and tokenized") {
    std::string code =
        "import if else for while do break continue return struct class public private "
        "new delete true false null const void match enum int float double char string bool "
        "i8 i16 i32 i64 i128 u8 u16 u32 u64 u128 f32 f64";

    Scanner scanner(code);
    auto tokens = scanner.get_tokens();

    std::vector<TokenType> expected = {
        TokenType::TOKEN_IMPORT, TokenType::TOKEN_IF, TokenType::TOKEN_ELSE, TokenType::TOKEN_FOR,
        TokenType::TOKEN_WHILE, TokenType::TOKEN_DO, TokenType::TOKEN_BREAK, TokenType::TOKEN_CONTINUE,
        TokenType::TOKEN_RETURN, TokenType::TOKEN_STRUCT, TokenType::TOKEN_CLASS, TokenType::TOKEN_PUBLIC,
        TokenType::TOKEN_PRIVATE, TokenType::TOKEN_NEW, TokenType::TOKEN_DELETE, TokenType::TOKEN_TRUE,
        TokenType::TOKEN_FALSE, TokenType::TOKEN_NULL, TokenType::TOKEN_CONST, TokenType::TOKEN_VOID,
        TokenType::TOKEN_MATCH, TokenType::TOKEN_ENUM, TokenType::TOKEN_INT, TokenType::TOKEN_FLOAT,
        TokenType::TOKEN_DOUBLE, TokenType::TOKEN_CHAR, TokenType::TOKEN_STRING, TokenType::TOKEN_BOOL,
        TokenType::TOKEN_I8, TokenType::TOKEN_I16, TokenType::TOKEN_I32, TokenType::TOKEN_I64,
        TokenType::TOKEN_I128, TokenType::TOKEN_U8, TokenType::TOKEN_U16, TokenType::TOKEN_U32,
        TokenType::TOKEN_U64, TokenType::TOKEN_U128, TokenType::TOKEN_F32, TokenType::TOKEN_F64,
        TokenType::TOKEN_EOF
    };

    REQUIRE(tokens.size() == expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        REQUIRE(tokens[i].type == expected[i]);
    }
}