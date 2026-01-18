#include "lexer.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const struct {
    const char* name;
    TokenType type;
} keywords[] = {
    /* Keywords */
    {"import", TOKEN_IMPORT},
    {"include", TOKEN_INCLUDE},
    {"if", TOKEN_IF},
    {"else", TOKEN_ELSE},
    {"for", TOKEN_FOR},
    {"while", TOKEN_WHILE},
    {"break", TOKEN_BREAK},
    {"continue", TOKEN_CONTINUE},
    {"return", TOKEN_RETURN},
    {"struct", TOKEN_STRUCT},
    {"class", TOKEN_CLASS},
    {"public", TOKEN_PUBLIC},
    {"private", TOKEN_PRIVATE},
    {"new", TOKEN_NEW},
    {"delete", TOKEN_DELETE},
    {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE},
    {"null", TOKEN_NULL},
    {"const", TOKEN_CONST},
    {"void", TOKEN_VOID},
    {"match", TOKEN_MATCH},
    {"enum", TOKEN_ENUM},

    /* Types */
    {"int", TOKEN_INT},
    {"float", TOKEN_FLOAT},
    {"double", TOKEN_DOUBLE},
    {"char", TOKEN_CHAR},
    {"string", TOKEN_STRING},
    {"bool", TOKEN_BOOL},
    {"i8", TOKEN_I8},
    {"i16", TOKEN_I16},
    {"i32", TOKEN_I32},
    {"i64", TOKEN_I64},
    {"u8", TOKEN_U8},
    {"u16", TOKEN_U16},
    {"u32", TOKEN_U32},
    {"u64", TOKEN_U64},
    {"f8", TOKEN_F8},
    {"f16", TOKEN_F16},
    {"f32", TOKEN_F32},
    {"f64", TOKEN_F64},
};

static char next_char(Lexer* lx) {
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

static Token make_token(Lexer* lx, TokenType type, size_t start, size_t length, char* lexeme) {
    Token tk;
    tk.lexeme = lexeme;
    tk.type = type;
    tk.start = start;
    tk.length = length;
    tk.line = lx->line;
    tk.column = lx->column - (lx->pos - start);
    return tk;
}

TokenType is_keyword(const char* str) {
    size_t len = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < len; i++) {
        if (strcmp(str, keywords[i].name) == 0) {
            return keywords[i].type;
        }
    }
    return TOKEN_IDENT;
}

static void skip_untracked(Lexer* lx) {
    while (lx->pos < lx->length) {
        char c = peek_char(lx);

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            next_char(lx);
            continue;
        }

        if (c == '/' && peek_next_char(lx) == '/') {
            while (peek_char(lx) != '\n' && peek_char(lx) != '\0') {
                next_char(lx);
            }
            continue;
        }

        if (c == '/' && peek_next_char(lx) == '*') {
            next_char(lx);
            next_char(lx);
            while (peek_char(lx) && !(peek_char(lx) == '*' && peek_next_char(lx) == '/')) {
                next_char(lx);
            }
            if (peek_char(lx)) {
                next_char(lx);
                next_char(lx);
            } else {
                errors_add(lx->errors, NULL, "Unterminated block comment", SEVERITY_ERROR, lx->line, lx->column, 0, CATEGORY_LEXER);
            }
            continue;
        }
        break;
    }
}

static Token lex_identifier(Lexer* lx) {
    size_t start = lx->pos;
    while (lx->pos < lx->length && is_alpha_numeric(lx->src[lx->pos])) {
        next_char(lx);
    }
    size_t len = lx->pos - start;

    char* s = (char*)malloc(len + 1);
    if (!s) {
        return make_token(lx, TOKEN_ERROR, start, 0, NULL);
    }
    memcpy(s, lx->src + start, len);
    s[len] = '\0';

    TokenType tt = is_keyword(s);

    Token t = make_token(lx, tt, start, len, s);
    return t;
}

static Token lex_number(Lexer* lx) {
    size_t start = lx->pos;
    int is_float = 0;
    while (lx->pos < lx->length) {
        char c = peek_char(lx);
        if (isdigit(c)) {
            next_char(lx);
        } else if (c == '.') {
            if (is_float) {
                errors_add(lx->errors, lx->filename, "Too many decimal points in number", SEVERITY_ERROR, lx->line,
                           lx->column, 0, CATEGORY_LEXER);
                break;
            }
            if (isdigit(peek_next_char(lx))) {
                is_float = 1;
                next_char(lx);
            } else {
                break;
            }
        } else {
            break;
        }
    }
    size_t length = lx->pos - start;
    char* lexeme = (char*)malloc(length + 1);
    if (!lexeme) {
        return make_token(lx, TOKEN_ERROR, start, 0, NULL);
    }
    memcpy(lexeme, lx->src + start, length);
    lexeme[length] = '\0';

    TokenType type;
    if (is_float) {
        type = TOKEN_FLOAT_LITERAL;
    } else {
        type = TOKEN_INT_LITERAL;
    }
    return make_token(lx, type, start, length, lexeme);
}

static Token lex_char(Lexer* lx) {
    size_t start = lx->pos;
    next_char(lx);

    char ch = '\0';

    if (peek_char(lx) == '\\') {
        next_char(lx);
        char next = peek_char(lx);
        switch (next) {
            case '\\':
                ch = '\\';
                break;
            case '\'':
                ch = '\'';
                break;
            case '"':
                ch = '"';
                break;
            case 'n':
                ch = '\n';
                break;
            case 't':
                ch = '\t';
                break;
            case 'r':
                ch = '\r';
                break;
            case '0':
                ch = '\0';
                break;
            default:
                ch = next;
                break;
        }
        next_char(lx);
    } else if (peek_char(lx) != '\'' && peek_char(lx) != '\0') {
        ch = peek_char(lx);
        next_char(lx);
    }

    if (peek_char(lx) == '\'') {
        next_char(lx);
    } else {
        errors_add(lx->errors, lx->filename, "Unterminated character literal", SEVERITY_ERROR, lx->line, lx->column,
                   0, CATEGORY_LEXER);
    }

    size_t length = lx->pos - start;
    char* lexeme = malloc(2);
    if (!lexeme) {
        return make_token(lx, TOKEN_ERROR, start, 0, NULL);
    }
    lexeme[0] = ch;
    lexeme[1] = '\0';

    return make_token(lx, TOKEN_CHAR_LITERAL, start, length, lexeme);
}

static Token lex_string(Lexer* lx) {
    size_t start = lx->pos;
    next_char(lx);

    size_t len = 0;
    size_t pos = lx->pos;

    while (pos < lx->length && lx->src[pos] != '"') {
        if (lx->src[pos] == '\\') {
            pos++;
            if (pos < lx->length) {
                len++;
                pos++;
            }
        } else {
            len++;
            pos++;
        }
    }

    char* buffer = malloc(len + 1);
    if (!buffer) {
        return make_token(lx, TOKEN_ERROR, start, 0, NULL);
    }
    size_t i = 0;

    while (lx->pos < lx->length && peek_char(lx) != '"') {
        if (peek_char(lx) == '\\') {
            next_char(lx);
            if (lx->pos < lx->length) {
                char next = peek_char(lx);
                switch (next) {
                    case 'n':
                        buffer[i++] = '\n';
                        break;
                    case 't':
                        buffer[i++] = '\t';
                        break;
                    case 'r':
                        buffer[i++] = '\r';
                        break;
                    case '0':
                        buffer[i++] = '\0';
                        break;
                    case '\\':
                        buffer[i++] = '\\';
                        break;
                    case '"':
                        buffer[i++] = '"';
                        break;
                    case '\'':
                        buffer[i++] = '\'';
                        break;
                    default:
                        buffer[i++] = next;
                        break;
                }
                next_char(lx);
            }
        } else {
            buffer[i++] = peek_char(lx);
            next_char(lx);
        }
    }

    if (peek_char(lx) == '\"') {
        next_char(lx);
        buffer[i] = '\0';

        size_t length = lx->pos - start;
        return make_token(lx, TOKEN_STRING_LITERAL, start, length, buffer);
    } else {
        free(buffer);
        errors_add(lx->errors, lx->filename, "Unterminated string literal", SEVERITY_ERROR, lx->line, lx->column, 0,
                   CATEGORY_LEXER);
        return make_token(lx, TOKEN_ERROR, start, lx->pos - start, NULL);
    }
}

static Token lex_operator(Lexer* lx) {
    size_t start = lx->pos;
    char c = next_char(lx);

    char* lexeme = (char*)malloc(4);
    if (!lexeme) {
        return make_token(lx, TOKEN_ERROR, start, 0, NULL);
    }
    lexeme[0] = c;
    lexeme[1] = '\0';
    size_t length = 1;

    switch (c) {
        case '(':
            return make_token(lx, TOKEN_LPAREN, start, length, lexeme);
        case ')':
            return make_token(lx, TOKEN_RPAREN, start, length, lexeme);
        case '{':
            return make_token(lx, TOKEN_LBRACE, start, length, lexeme);
        case '}':
            return make_token(lx, TOKEN_RBRACE, start, length, lexeme);
        case '[':
            return make_token(lx, TOKEN_LBRACKET, start, length, lexeme);
        case ']':
            return make_token(lx, TOKEN_RBRACKET, start, length, lexeme);
        case ';':
            return make_token(lx, TOKEN_SEMICOLON, start, length, lexeme);
        case ',':
            return make_token(lx, TOKEN_COMMA, start, length, lexeme);
        case ':': {
            if (match_char(lx, ':')) {
                lexeme[1] = ':';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_DOUBLE_COLON, start, length, lexeme);
            }
            return make_token(lx, TOKEN_COLON, start, length, lexeme);
        }
        case '?':
            return make_token(lx, TOKEN_QUESTION, start, length, lexeme);
        case '@':
            return make_token(lx, TOKEN_AT, start, length, lexeme);
        case '~':
            return make_token(lx, TOKEN_TILDE, start, length, lexeme);
        case '^': {
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_XOR_EQUALS, start, length, lexeme);
            }
            return make_token(lx, TOKEN_CARET, start, length, lexeme);
        }
        case '.': {
            if ((peek_char(lx) == '.') && (peek_next_char(lx) == '.')) {
                next_char(lx);
                next_char(lx);
                lexeme[1] = '.';
                lexeme[2] = '.';
                lexeme[3] = '\0';
                length = 3;
                return make_token(lx, TOKEN_ELLIPSIS, start, length, lexeme);
            }
            return make_token(lx, TOKEN_DOT, start, length, lexeme);
        }
        case '+': {
            if (match_char(lx, '+')) {
                lexeme[1] = '+';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_PLUS_PLUS, start, length, lexeme);
            }
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_PLUS_ASSIGN, start, length, lexeme);
            }
            return make_token(lx, TOKEN_PLUS, start, length, lexeme);
        }
        case '-': {
            if (match_char(lx, '-')) {
                lexeme[1] = '-';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_MINUS_MINUS, start, length, lexeme);
            }
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_MINUS_ASSIGN, start, length, lexeme);
            }
            if (match_char(lx, '>')) {
                lexeme[1] = '>';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_ARROW, start, length, lexeme);
            }
            return make_token(lx, TOKEN_MINUS, start, length, lexeme);
        }
        case '*': {
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_STAR_ASSIGN, start, length, lexeme);
            }
            return make_token(lx, TOKEN_STAR, start, length, lexeme);
        }
        case '/': {
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_SLASH_ASSIGN, start, length, lexeme);
            }
            return make_token(lx, TOKEN_SLASH, start, length, lexeme);
        }
        case '%': {
            return make_token(lx, TOKEN_PERCENT, start, length, lexeme);
        }
        case '=': {
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_EQUAL, start, length, lexeme);
            }
            if (match_char(lx, '>')) {
                lexeme[1] = '>';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_FAT_ARROW, start, length, lexeme);
            }
            return make_token(lx, TOKEN_ASSIGN, start, length, lexeme);
        }
        case '!': {
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_NOT_EQUAL, start, length, lexeme);
            }
            return make_token(lx, TOKEN_LOGICAL_NOT, start, length, lexeme);
        }
        case '<': {
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_LESS_EQUAL, start, length, lexeme);
            }
            if (match_char(lx, '<')) {
                if (match_char(lx, '=')) {
                    lexeme[1] = '<';
                    lexeme[2] = '=';
                    lexeme[3] = '\0';
                    length = 3;
                    return make_token(lx, TOKEN_LSHIFT_EQUALS, start, length, lexeme);
                }
                lexeme[1] = '<';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_LEFT_SHIFT, start, length, lexeme);
            }
            return make_token(lx, TOKEN_LESS, start, length, lexeme);
        }
        case '>': {
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_GREATER_EQUAL, start, length, lexeme);
            }
            if (match_char(lx, '>')) {
                if (match_char(lx, '=')) {
                    lexeme[1] = '>';
                    lexeme[2] = '=';
                    lexeme[3] = '\0';
                    length = 3;
                    return make_token(lx, TOKEN_RSHIFT_EQUALS, start, length, lexeme);
                }
                lexeme[1] = '>';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_RIGHT_SHIFT, start, length, lexeme);
            }
            return make_token(lx, TOKEN_GREATER, start, length, lexeme);
        }
        case '&': {
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_AND_EQUALS, start, length, lexeme);
            }
            if (match_char(lx, '&')) {
                lexeme[1] = '&';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_LOGICAL_AND, start, length, lexeme);
            }
            return make_token(lx, TOKEN_AMPERSAND, start, length, lexeme);
        }
        case '|': {
            if (match_char(lx, '=')) {
                lexeme[1] = '=';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_OR_EQUALS, start, length, lexeme);
            }
            if (match_char(lx, '|')) {
                lexeme[1] = '|';
                lexeme[2] = '\0';
                length = 2;
                return make_token(lx, TOKEN_LOGICAL_OR, start, length, lexeme);
            }
            return make_token(lx, TOKEN_PIPE, start, length, lexeme);
        }
        default:
            return make_token(lx, TOKEN_ERROR, start, length, lexeme);
    }
}

/* Main */
static Token next_token(Lexer* lx) {
    skip_untracked(lx);

    if (lx->pos >= lx->length) {
        return make_token(lx, TOKEN_EOF, lx->pos, 0, NULL);
    }

    char c = peek_char(lx);

    if (isdigit(c)) {
        return lex_number(lx);
    }

    if (isalpha(c) || c == '_') {
        return lex_identifier(lx);
    }

    if (c == '"') {
        return lex_string(lx);
    }
    if (c == '\'') {
        return lex_char(lx);
    }

    return lex_operator(lx);
}

Lexer* init_lexer(const char* filename, const char* src, ErrorList* error_list) {
    Lexer* lx = (Lexer*)malloc(sizeof(Lexer));
    if (!lx)
        return NULL;
    lx->errors = error_list;
    lx->src = src;
    lx->filename = filename;
    lx->length = strlen(src);
    lx->pos = 0;
    lx->line = 1;
    lx->column = 1;
    return lx;
}

void free_lexer(Lexer* lx) {
    free(lx);
}

Token* run_lexer(Lexer* lx, size_t* count) {
    size_t max = 16;
    size_t n = 0;
    Token* tokens = (Token*)malloc(sizeof(Token) * max);

    for (;;) {
        if (n >= max) {
            max = max * 2;
            tokens = (Token*)realloc(tokens, sizeof(Token) * max);
        }
        Token t = next_token(lx);
        tokens[n++] = t;
        if (t.type == TOKEN_EOF) {
            break;
        }
    }
    *count = n;
    return tokens;
}

void free_token(Token* t) {
    if (t->lexeme)
        free(t->lexeme);
}
