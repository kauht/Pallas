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

typedef struct ASTNode ASTNode;

typedef struct ASTNode {
    ASTNodeType type;
    size_t start, length, line, column;

    union {
        struct {
            ASTNode* left;
            ASTNode* right;
            uint32_t line, column;
        } binary;
        struct {
            ASTNode* operand;
        } unary;
        struct {
            char* value;
        } literal;
        struct {
            ASTNode** statements;
            int count;
        } block;
        struct {
            char* name;
            ASTNode* params;
            ASTNode* body;
        } function;
    };
} ASTNode;
#endif /* AST_H */
