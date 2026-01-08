#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>

#include "../lex/lexer.h"
#include "ast.h"
#include "diagnostic.h"

typedef struct Parser {
    Token* tokens;
    uint32_t current;
    uint32_t count;

    ErrorList* errors;
    bool panic;
} Parser;

typedef enum {
    TYPE_VOID,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_BOOL,

    /* Explicitly Sized Types */
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_F32,
    TYPE_F64,

    /* Other */
    TYPE_POINTER,
    TYPE_ARRAY,
    TYPE_USER,
    TYPE_STRUCT,
    TYPE_CLASS
} TypeKind;

typedef struct Type {
    TypeKind kind;
    struct Type* base;
    uint32_t arr_size;
    char* name;
    struct Type* return_type;
    struct Type** param_types;
    uint32_t param_count;
} Type;

Parser* init_parser(Token* tks, uint32_t count, ErrorList* error_list);

ASTNode* run_parser(Parser* parser);

ASTNode* create_ast_node(ASTNodeType type, ASTNode* left, ASTNode* right, uint32_t line,
                         uint32_t column);

#endif /* PARSER_H */
