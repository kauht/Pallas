#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdint.h>

#include "../lex/lexer.h"

typedef enum {
    AST_UNKNOWN = 0,
    AST_PROGRAM,
    AST_FUNCTION,
    AST_VARIABLE,
    AST_BLOCK,
    AST_EXPR_STMT,
    AST_RETURN,
    AST_IF,
    AST_FOR,
    AST_WHILE,
    AST_BREAK,
    AST_CONTINUE,
    AST_EXPR,
    AST_LITERAL,
    AST_IDENT,
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    size_t start;
    size_t length;
    size_t line;
    size_t column;

    struct ASTNode* first;
    struct ASTNode* next;
    void* payload;
} ASTNode;

typedef struct Parser {
    Lexer* lexer;
    Token current;
    Token lookahead;
    char* error_message;
    int panic;
} Parser;

#endif /* PARSER_H */
