#pragma once

#include <string>
#include <unordered_map>
#include "../filesystem/fs.h"

enum TokenType {
    TOKEN_EOF,
    TOKEN_IDENT,

    TOKEN_IMPORT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
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

    TOKEN_ASSIGN,
    TOKEN_ARROW,

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
    TOKEN_TILDE
};

struct Token {
    std::string lexeme;
    uint32_t line;
    uint32_t column;
    TokenType type;
};

class Lexer {
  public:
    Lexer(const std::string& path);
    ~Lexer();
    void run();

  private:
    uint32_t position;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> keywords;
    File file;

    // Helper
    bool at_end();
    char next_char();
    char peek_char();
    Token create_token(std::string lexeme, uint32_t line, uint32_t column, TokenType type);
    TokenType is_keyword(std::string s);
    void skip_untracked();

    // Lexer
    Token lex_identifier();
    Token lex_number();
    Token lex_char();
    Token lex_string();
    Token lex_operator();

    // Main
    Token next_token();
};
