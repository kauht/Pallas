#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "diagnostics.h"

namespace pallas::frontend {

enum class TokenType : std::uint8_t {
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_IMPORT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_RETURN,
    TOKEN_STRUCT,
    TOKEN_CLASS,
    TOKEN_PUBLIC,
    TOKEN_PRIVATE,
    TOKEN_NEW,
    TOKEN_DELETE,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,
    TOKEN_CONST,
    TOKEN_VOID,
    TOKEN_MATCH,
    TOKEN_ENUM,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_I8,
    TOKEN_I16,
    TOKEN_I32,
    TOKEN_I64,
    TOKEN_I128,
    TOKEN_U8,
    TOKEN_U16,
    TOKEN_U32,
    TOKEN_U64,
    TOKEN_U128,
    TOKEN_F32,
    TOKEN_F64,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_DOUBLE,
    TOKEN_CHAR,
    TOKEN_STRING,
    TOKEN_BOOL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_DOT,
    TOKEN_ELLIPSIS,
    TOKEN_QUESTION,
    TOKEN_AT,
    TOKEN_DOUBLE_COLON,
    TOKEN_ARROW,
    TOKEN_ASSIGN,
    TOKEN_PLUS_ASSIGN,
    TOKEN_MINUS_ASSIGN,
    TOKEN_STAR_ASSIGN,
    TOKEN_SLASH_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_PLUS_PLUS,
    TOKEN_MINUS_MINUS,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LOGICAL_AND,
    TOKEN_LOGICAL_OR,
    TOKEN_LOGICAL_NOT,
    TOKEN_AMPERSAND,
    TOKEN_PIPE,
    TOKEN_CARET,
    TOKEN_TILDE,
    TOKEN_LEFT_SHIFT,
    TOKEN_RIGHT_SHIFT,
    TOKEN_IDENT
};

struct Token {
    TokenType type = TokenType::TOKEN_ERROR;
    std::string lexeme;
    std::size_t length = 0;
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;
};

class Scanner {
  private:
    std::unordered_map<std::string, TokenType> keywords = {
        {"import", TokenType::TOKEN_IMPORT},   {"if", TokenType::TOKEN_IF},
        {"else", TokenType::TOKEN_ELSE},       {"for", TokenType::TOKEN_FOR},
        {"while", TokenType::TOKEN_WHILE},     {"do", TokenType::TOKEN_DO},
        {"break", TokenType::TOKEN_BREAK},     {"continue", TokenType::TOKEN_CONTINUE},
        {"return", TokenType::TOKEN_RETURN},   {"struct", TokenType::TOKEN_STRUCT},
        {"class", TokenType::TOKEN_CLASS},     {"public", TokenType::TOKEN_PUBLIC},
        {"private", TokenType::TOKEN_PRIVATE}, {"new", TokenType::TOKEN_NEW},
        {"delete", TokenType::TOKEN_DELETE},   {"true", TokenType::TOKEN_TRUE},
        {"false", TokenType::TOKEN_FALSE},     {"null", TokenType::TOKEN_NULL},
        {"const", TokenType::TOKEN_CONST},     {"void", TokenType::TOKEN_VOID},
        {"match", TokenType::TOKEN_MATCH},     {"enum", TokenType::TOKEN_ENUM},
        {"int", TokenType::TOKEN_INT},         {"float", TokenType::TOKEN_FLOAT},
        {"double", TokenType::TOKEN_DOUBLE},   {"char", TokenType::TOKEN_CHAR},
        {"string", TokenType::TOKEN_STRING},   {"bool", TokenType::TOKEN_BOOL},
        {"i8", TokenType::TOKEN_I8},           {"i16", TokenType::TOKEN_I16},
        {"i32", TokenType::TOKEN_I32},         {"i64", TokenType::TOKEN_I64},
        {"i128", TokenType::TOKEN_I128},       {"u8", TokenType::TOKEN_U8},
        {"u16", TokenType::TOKEN_U16},         {"u32", TokenType::TOKEN_U32},
        {"u64", TokenType::TOKEN_U64},         {"u128", TokenType::TOKEN_U128},
        {"f32", TokenType::TOKEN_F32},         {"f64", TokenType::TOKEN_F64},
    };

    Scanner(std::string source_text);
    Token next_token();

    std::string source;
    std::size_t start = 0;
    std::size_t position = 0;
    std::size_t line = 1;
    std::size_t column = 1;
    std::size_t start_line = 1;
    std::size_t start_column = 1;
    std::size_t token_index = 0;
    std::vector<Token> tokens;
    Diagnostics* diagnostics = nullptr;

    void report(Severity sev, ErrorCode code, const std::string& msg, std::size_t start_offset,
                std::size_t pos_offset, std::size_t line_no, std::size_t col_no);
    bool is_at_end();
    char advance();
    bool match_char(char expected);
    char peek_char();
    char peek_char(uint8_t n);
    void add_token(TokenType type);
    Token make_token(TokenType type);
    TokenType is_keyword(const std::string& ident);
    void skip_untracked();
    void s_identifier();
    void s_number();
    void s_char();
    void s_string();
    void s_operator();
    void scan();
};
}  // namespace pallas::frontend
