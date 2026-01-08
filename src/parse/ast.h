#ifndef AST_H
#define AST_H
#include <stddef.h>
#include <stdint.h>

typedef enum {
    AST_UNKNOWN = 0,
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

    struct ASTNode* left;
    struct ASTNode* right;
    void* payload;
} ASTNode;
#endif /* AST_H */
