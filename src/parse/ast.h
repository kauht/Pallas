#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdint.h>

typedef struct ASTNode ASTNode;

typedef enum ASTNodeType {
    AST_UNKNOWN = 0,
    AST_PROGRAM,

    /* Declarations */
    AST_IMPORT,
    AST_FUN_DECL,
    AST_VAR_DECL,
    AST_STRUCT_DECL,
    AST_CLASS_DECL,
    AST_PARAM,
    AST_PARAM_LIST,
    AST_STRUCT_MEMBER,
    AST_CLASS_MEMBER,

    /* Types */
    AST_TYPE_BUILTIN,
    AST_TYPE_USER,
    AST_TYPE_POINTER,

    /* Statements */
    AST_BLOCK,
    AST_IF,
    AST_WHILE,
    AST_FOR,
    AST_RETURN,
    AST_EXPR_STATEMENT,

    /* Expressions */
    AST_ASSIGNMENT,
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_IDENTIFIER,
    AST_INT_LITERAL,
    AST_FLOAT_LITERAL,
    AST_CHAR_LITERAL,
    AST_STRING_LITERAL
} ASTNodeType;

struct ASTNode {
    ASTNodeType type;
    size_t start;
    struct {
        size_t length;
        size_t line;
        size_t column;
    } position;

    union {
        /* Program -> Import* TopDecl* */
        struct {
            ASTNode** imports;
            size_t import_count;
            size_t import_capacity;

            ASTNode** top_decls; /* FunDecl | StructDecl | ClassDecl | VarDecl */
            size_t top_decl_count;
            size_t top_decl_capacity;
        } program;

        /* Import -> import IDENT ('.' IDENT)* ';' */
        struct {
            char** segments; /* dotted path pieces */
            size_t segment_count;
            size_t segment_capacity;
        } import_node;

        /* FunDecl -> IDENT '(' ParamList? ')' ':' Type Block */
        struct {
            char* name;
            ASTNode* params;      /* AST_PARAM_LIST or NULL */
            ASTNode* return_type; /* AST_TYPE_* */
            ASTNode* body;        /* AST_BLOCK */
        } function_declaration;

        /* ParamList -> Param (',' Param)* */
        struct {
            ASTNode** params; /* AST_PARAM */
            size_t param_count;
            size_t param_capacity;
        } parameter_list;

        /* Param -> IDENT ':' Type */
        struct {
            char* name;
            ASTNode* type; /* AST_TYPE_* */
        } parameter;

        /* VarDecl -> IDENT ':' Type ('=' Expression)? ';' */
        struct {
            char* name;
            ASTNode* type;        /* AST_TYPE_* */
            ASTNode* initializer; /* expression or NULL */
        } variable_declaration;

        /* StructDecl -> struct IDENT '{' (VarDecl | FunDecl)* '}' */
        struct {
            char* name;
            ASTNode** members; /* VarDecl or FunDecl, or struct_member */
            size_t member_count;
            size_t member_capacity;
        } struct_declaration;

        /* ClassDecl -> class IDENT '{' (VarDecl | FunDecl)* '}' */
        struct {
            char* name;
            ASTNode** members; /* VarDecl or FunDecl, or class_member */
            size_t member_count;
            size_t member_capacity;
        } class_declaration;

        struct {
            ASTNode* declaration; /* VarDecl or FunDecl */
        } struct_member;

        struct {
            ASTNode* declaration; /* VarDecl or FunDecl */
        } class_member;

        /* Block -> '{' ( VarDecl | Statement )* '}' */
        struct {
            ASTNode** items; /* VarDecl or any Statement */
            size_t item_count;
            size_t item_capacity;
        } block;

        /* IfStmt */
        struct {
            ASTNode* condition;
            ASTNode* then_block;  /* BLOCK */
            ASTNode* else_branch; /* NULL, BLOCK, or IF */
        } if_statement;

        /* WhileStmt */
        struct {
            ASTNode* condition;
            ASTNode* body; /* BLOCK */
        } while_statement;

        /* ForStmt */
        struct {
            ASTNode* init;      /* VarDecl, ExprStatement, or NULL */
            ASTNode* condition; /* expression or NULL */
            ASTNode* post;      /* expression or NULL */
            ASTNode* body;      /* BLOCK */
        } for_statement;

        /* ReturnStmt */
        struct {
            ASTNode* expression; /* expression or NULL */
        } return_statement;

        /* ExpressionStmt */
        struct {
            ASTNode* expression;
        } expression_statement;

        /* Types */

        /* BuiltinType / SizedType: instead of an enum, just keep the token kind as int */
        struct {
            int builtin_kind; /* TOKEN_INT, TOKEN_I32, etc. */
        } builtin_type;

        struct {
            char* name;
        } user_type;

        struct {
            ASTNode* base; /* type of pointer */
        } pointer_type;

        /* Assignment: LEFT = RIGHT */
        struct {
            ASTNode* target; /* Left */
            ASTNode* value;  /* Right */
        } assignment;

        struct {
            int op;
            ASTNode* left;
            ASTNode* right;
        } binary_expression;

        struct {
            int op;
            ASTNode* operand;
        } unary_expression;

        struct {
            char* name;
        } identifier;

        /* Literals */
        struct {
            int64_t value;
            char* lexeme;
        } int_literal;

        struct {
            double value;
            char* lexeme;
        } float_literal;

        struct {
            char value;
            char* lexeme;
        } char_literal;

        struct {
            char* value;
        } string_literal;
    };
};

#endif /* AST_H */
