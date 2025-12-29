#include "lexer.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const struct {
    const char* name;
    TokenType type;
} keywords[] = {
    /* keywords */
    {"import", TOKEN_IMPORT},
    {"if", TOKEN_IF},
    {"else", TOKEN_ELSE},
    {"for", TOKEN_FOR},
    {"while", TOKEN_WHILE},
    {"break", TOKEN_BREAK},
    {"continue", TOKEN_CONTINUE},
    {"return", TOKEN_RETURN},
    {"struct", TOKEN_STRUCT},
    {"class", TOKEN_CLASS},
    {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE},
    {"null", TOKEN_NULL},

    /* Types */
    {"int", TOKEN_INT},
    {"float", TOKEN_FLOAT},
    {"char", TOKEN_CHAR},
    {"string", TOKEN_STRING},

    /* Sized Types */
    {"i32", TOKEN_I32},
    {"i64", TOKEN_I64},
    {"u32", TOKEN_U32},
    {"u64", TOKEN_U64},
    {"f32", TOKEN_F32},
    {"f64", TOKEN_F64},
};

// Helper Functions

TokenType is_keyword(const char* str) {
    size_t len = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < len; i++) {
        if (strcmp(str, keywords[i].name) == 0) {
            return keywords[i].type;
        }
    }
    return TOKEN_IDENT;
}

static char peek_char(Lexer* lx) {
    if (lx->pos >= lx->length) {
        return '\0';
    }
    return lx->src[lx->pos];
}

static char peek_next_char(Lexer* lx) {
    if (lx->pos + 1 >= lx->length) {
        return '\0';
    }
    return lx->src[lx->pos + 1];
}

static int match_char(Lexer* lx, char expected) {
    if (peek_char(lx) == expected) {
        next_char(lx);
        return 1;
    }
    return 0;
}

static int is_alpha_numeric(char c) {
    if (isalnum(c) || c == '_') {
        return 1;
    }
    return 0;
}

static Token make_token(Lexer* lx, TokenType type, size_t start, size_t length, char* literal) {
    Token tk;
    tk.type = type;
    tk.start = start;
    tk.length = length;
    tk.lexeme = lx->src + start;
    tk.literal = literal;
    tk.line = lx->line;
    tk.column = lx->column - (lx->pos - start);
    return tk;
}

static void skip_untracked(Lexer* lx) {
    while (lx->pos < lx->length) {
        char c = peek_char(lx);

        // White space
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            next_char(lx);
            continue;
        }

        // Single-line comment
        if (c == '/' && peek_next_char(lx) == '/') {
            while (peek_char(lx) != '\n' && peek_char(lx) != '\0') {
                next_char(lx);
            }
            continue;
        }

        // Multi-line comment
        if (c == '/' && peek_next_char(lx) == '*') {
            next_char(lx);
            next_char(lx);
            while (peek_char(lx) && !(peek_char(lx) == '*' && peek_next_char(lx) == '/')) {
                next_char(lx);
            }
            if (peek_char(lx)) {
                next_char(lx);
                next_char(lx);
            }
            continue;
        }
        break;
    }
}

// Type-specific
static Token lex_identifier(Lexer* lx) {
    size_t start = lx->pos;
    while (isalpha(lx->src[lx->pos]) || lx->src[lx->pos] == '_') {
        next_char(lx);
    }
    size_t len = lx->pos - start;

    const char* s = (char*)malloc(sizeof(len));
    // memcpy(start, s, len);

    // If matches with keyword list, return respective token type, else return as
    // TOKEN_IDENT
}
static Token lex_number(Lexer* lx) {
    size_t start = lx->pos;
    int dec = 0;
    while (isdigit(lx->src[lx->pos])) {
        if (!dec) {
        }
    }
    // const char* literal = (char*)malloc();
    // return make_token(lx, TOKEN_FLOAT, start, , char* literal);
    //  Check if float/int type
}
static Token lex_string(Lexer* lx) {
}
static Token lex_operator(Lexer* lx) {
}

Lexer* init_lexer(const char* src) {
    Lexer* lx = (Lexer*)malloc(sizeof(Lexer));
    if (!lx) return NULL;
    lx->src = src;
    lx->length = strlen(src);
    lx->pos = 0;
    lx->line = 1;
    lx->column = 1;
    return lx;
}

void free_lexer(Lexer* lx) {
    free(lx);
}

char next_char(Lexer* lx) {
    char c = lx->src[lx->pos];
    lx->pos++;
    if (c == '\n') {
        lx->line++;
        lx->column = 1;
    } else {
        lx->column++;
    }
    return c;
}

Token next_token(Lexer* lx) {
    skip_untracked(lx);

    if (lx->pos >= lx->length) {
        // make token EOF
    }

    while (lx->pos < lx->length) {
        char c = lx->src[lx->pos];

        // Number
        if (isdigit(c)) {
        }

        // String
        if (c == '"') {
            next_char(lx);
            while (lx->pos < lx->length) {
                if (lx->src[lx->pos] == '"') {
                }
            }
        }

        break;
    }
}

void free_token(Token* t) {
    if (t->literal) free(t->literal);
}
