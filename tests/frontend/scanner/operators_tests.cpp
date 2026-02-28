#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <string>
#include "frontend/scanner.h"

using namespace pallas::frontend;

static std::vector<Token> non_eof_tokens(const std::vector<Token>& toks) {
    std::vector<Token> out;
    out.reserve(toks.size());
    for (const auto &t : toks) {
        if (t.type == TokenType::TOKEN_EOF) break;
        out.push_back(t);
    }
    return out;
}

TEST_CASE("operators: single and compound tokens") {
    std::string code = R"CODE(
        + ++ += - -- -= -> * *= / /= % ++ -- == != < <= > >= << >> && || ! & | ^ ~
        . ... ? @ : :: ; , ( ) { } [ ]
    )CODE";

    Scanner scanner(code);
    auto toks = non_eof_tokens(scanner.get_tokens());

    std::vector<TokenType> expected = {
        TokenType::TOKEN_PLUS,
        TokenType::TOKEN_PLUS_PLUS,
        TokenType::TOKEN_PLUS_ASSIGN,
        TokenType::TOKEN_MINUS,
        TokenType::TOKEN_MINUS_MINUS,
        TokenType::TOKEN_MINUS_ASSIGN,
        TokenType::TOKEN_ARROW,
        TokenType::TOKEN_STAR,
        TokenType::TOKEN_STAR_ASSIGN,
        TokenType::TOKEN_SLASH,
        TokenType::TOKEN_SLASH_ASSIGN,
        TokenType::TOKEN_PERCENT,
        TokenType::TOKEN_PLUS_PLUS,
        TokenType::TOKEN_MINUS_MINUS,
        TokenType::TOKEN_EQUAL,
        TokenType::TOKEN_NOT_EQUAL,
        TokenType::TOKEN_LESS,
        TokenType::TOKEN_LESS_EQUAL,
        TokenType::TOKEN_GREATER,
        TokenType::TOKEN_GREATER_EQUAL,
        TokenType::TOKEN_LEFT_SHIFT,
        TokenType::TOKEN_RIGHT_SHIFT,
        TokenType::TOKEN_LOGICAL_AND,
        TokenType::TOKEN_LOGICAL_OR,
        TokenType::TOKEN_LOGICAL_NOT,
        TokenType::TOKEN_AMPERSAND,
        TokenType::TOKEN_PIPE,
        TokenType::TOKEN_CARET,
        TokenType::TOKEN_TILDE,
        TokenType::TOKEN_DOT,
        TokenType::TOKEN_ELLIPSIS,
        TokenType::TOKEN_QUESTION,
        TokenType::TOKEN_AT,
        TokenType::TOKEN_COLON,
        TokenType::TOKEN_DOUBLE_COLON,
        TokenType::TOKEN_SEMICOLON,
        TokenType::TOKEN_COMMA,
        TokenType::TOKEN_LPAREN,
        TokenType::TOKEN_RPAREN,
        TokenType::TOKEN_LBRACE,
        TokenType::TOKEN_RBRACE,
        TokenType::TOKEN_LBRACKET,
        TokenType::TOKEN_RBRACKET
    };

    REQUIRE(toks.size() == expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        INFO("operator token index " << i << " lexeme='" << toks[i].lexeme << "'");
        REQUIRE(toks[i].type == expected[i]);
    }

    REQUIRE(toks[0].lexeme == "+");
    REQUIRE(toks[1].lexeme == "++");
    REQUIRE(toks[2].lexeme == "+=");
    REQUIRE(toks[6].lexeme == "->");
    REQUIRE(toks[29].lexeme == ".");
    REQUIRE(toks[30].lexeme == "...");
    REQUIRE(toks[34].lexeme == "::");
}

TEST_CASE("operators: shift and bitwise combinations") {
    std::string code = "a << b >> c & d | e ^ f ~g && h || i";
    Scanner scanner(code);
    auto toks = non_eof_tokens(scanner.get_tokens());

    std::vector<TokenType> expected = {
        TokenType::TOKEN_IDENT,
        TokenType::TOKEN_LEFT_SHIFT,
        TokenType::TOKEN_IDENT,
        TokenType::TOKEN_RIGHT_SHIFT,
        TokenType::TOKEN_IDENT,
        TokenType::TOKEN_AMPERSAND,
        TokenType::TOKEN_IDENT,
        TokenType::TOKEN_PIPE,
        TokenType::TOKEN_IDENT,
        TokenType::TOKEN_CARET,
        TokenType::TOKEN_IDENT,
        TokenType::TOKEN_TILDE,
        TokenType::TOKEN_IDENT,
        TokenType::TOKEN_LOGICAL_AND,
        TokenType::TOKEN_IDENT,
        TokenType::TOKEN_LOGICAL_OR,
        TokenType::TOKEN_IDENT
    };

    REQUIRE(toks.size() == expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        INFO("shift/bitwise token index " << i << " lexeme='" << toks[i].lexeme << "'");
        REQUIRE(toks[i].type == expected[i]);
    }

    REQUIRE(toks[1].lexeme == "<<");
    REQUIRE(toks[3].lexeme == ">>");
    REQUIRE(toks[5].lexeme == "&");
    REQUIRE(toks[11].lexeme == "~");
    REQUIRE(toks[13].lexeme == "&&");
}
