#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdint.h>

#include "diagnostic.h"

typedef enum {
    /* Special */
    TOKEN_EOF,
    TOKEN_ERROR,

    /* Keywords */
    TOKEN_IMPORT,
    TOKEN_INCLUDE,
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

    /* Literals */
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_STRING_LITERAL,

    /* Types */
    TOKEN_I8,
    TOKEN_I16,
    TOKEN_I32,
    TOKEN_I64,
    TOKEN_U8,
    TOKEN_U16,
    TOKEN_U32,
    TOKEN_U64,
    TOKEN_F8,
    TOKEN_F16,
    TOKEN_F32,
    TOKEN_F64,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_DOUBLE,
    TOKEN_CHAR,
    TOKEN_STRING,
    TOKEN_BOOL,

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
    TOKEN_QUESTION,     /* ? */
    TOKEN_AT,           /* @ */
    TOKEN_DOUBLE_COLON, /* :: */
    TOKEN_FAT_ARROW,    /* => */

    /* Assignment / Arrow */
    TOKEN_ASSIGN,      /* = */
    TOKEN_ARROW,       /* -> */
    TOKEN_PLUS_ASSIGN, /* += */
    TOKEN_MINUS_ASSIGN,
    TOKEN_STAR_ASSIGN,
    TOKEN_SLASH_ASSIGN,

    /* Arithmetic Operators */
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_PLUS_PLUS,
    TOKEN_MINUS_MINUS,

    /* Comparison Operators */
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,

    /* Logical Operators */
    TOKEN_LOGICAL_AND, /* && */
    TOKEN_LOGICAL_OR,  /* || */
    TOKEN_LOGICAL_NOT, /* ! */

    /* Other */
    TOKEN_AMPERSAND,     /* & */
    TOKEN_PIPE,          /* | */
    TOKEN_CARET,         /* ^ */
    TOKEN_TILDE,         /* ~ */
    TOKEN_LEFT_SHIFT,    /* << */
    TOKEN_RIGHT_SHIFT,   /* >> */
    TOKEN_AND_EQUALS,    /* &= */
    TOKEN_OR_EQUALS,     /* |= */
    TOKEN_XOR_EQUALS,    /* ^= */
    TOKEN_LSHIFT_EQUALS, /* <<= */
    TOKEN_RSHIFT_EQUALS, /* >>= */

    /* Identifiers */
    TOKEN_IDENT
} TokenType;

typedef struct {
    char* lexeme;
    size_t start;
    size_t length;
    size_t line;
    size_t column;
    TokenType type;
} Token;

typedef struct {
    const char* filename;
    const char* src;

    size_t length;
    size_t pos;
    size_t line;
    size_t column;

    ErrorList* errors;
} Lexer;

Lexer* init_lexer(const char* filename, const char* src, ErrorList* error_list);
Token* run_lexer(Lexer* lx, size_t* count);
void free_lexer(Lexer* lx);

void free_token(Token* t);

#endif /* LEXER_H */
