#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    /* Special */
    TOKEN_EOF,   /* End of file */
    TOKEN_ERROR, /* Unexpected character */

    /* Keywords */
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
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,

    /* Literals */
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_STRING,

    /* Explicit Types */
    TOKEN_I32,
    TOKEN_I64,
    TOKEN_U32,
    TOKEN_U64,
    TOKEN_F32,
    TOKEN_F64,

    /* Punctuation */
    TOKEN_LPAREN,    /* ( */
    TOKEN_RPAREN,    /* ) */
    TOKEN_LBRACE,    /* { */
    TOKEN_RBRACE,    /* } */
    TOKEN_LBRACKET,  /* [ */
    TOKEN_RBRACKET,  /* ] */
    TOKEN_SEMICOLON, /* ; */
    TOKEN_COMMA,     /* , */
    TOKEN_COLON,     /* : */
    TOKEN_DOT,       /* . */
    TOKEN_ELLIPSIS,  /* ... */

    /* Operators */
    TOKEN_PLUS,        /* + */
    TOKEN_MINUS,       /* - */
    TOKEN_STAR,        /* * */
    TOKEN_SLASH,       /* / */
    TOKEN_PERCENT,     /* % */
    TOKEN_PLUS_PLUS,   /* ++ */
    TOKEN_MINUS_MINUS, /* -- */

    /* Comparisons */
    TOKEN_EQUAL,         /* == */
    TOKEN_NOT_EQUAL,     /* != */
    TOKEN_LESS,          /* < */
    TOKEN_LESS_EQUAL,    /* <= */
    TOKEN_GREATER,       /* > */
    TOKEN_GREATER_EQUAL, /* >= */

    /* Logical and Bitwise */
    TOKEN_AND,         /* && */
    TOKEN_OR,          /* || */
    TOKEN_NOT,         /* ! */
    TOKEN_BITWISE_AND, /* & */
    TOKEN_BITWISE_OR,  /* | */
    TOKEN_BITWISE_XOR, /* ^ */
    TOKEN_BITWISE_NOT, /* ~ */
    TOKEN_LEFT_SHIFT,  /* << */
    TOKEN_RIGHT_SHIFT, /* >> */

    /* Misc */
    TOKEN_QUESTION, /* ? */
    TOKEN_ASSIGN,   /* = */
    TOKEN_ARROW,    /* -> */
    TOKEN_IDENT,
} TokenType;

typedef struct {
    const char* lexeme;
    size_t start;
    size_t length;
    char* literal;
    size_t line;
    size_t column;
    TokenType type;
} Token;

typedef struct {
    const char* src;

    size_t length;
    size_t pos;
    size_t line;
    size_t column;

    //ErrorList* s;
} Lexer;

Lexer* init_lexer(const char* src);
void free_lexer(Lexer* lx);

char next_char(Lexer* lx);   /* Consume and return the next char from the input
                                (ignoring   whitespace/comments). */
Token next_token(Lexer* lx); /* Consume input and return the next token */

void free_token(Token* t);

#endif /* LEXER_H */
