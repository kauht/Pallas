#include "parser.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"

/* Helper */

static bool at_end(Parser* parser) {

}

static void advance(Parser* parser) {
    parser->current++;
}

static Token peek(Parser* parser) {
    if (parser->current < parser->count) {
        return parser->tokens[parser->current];
    }
    return (Token){TOKEN_EOF, 0, 0, 0, 0, NULL};
}

static bool check(Parser* parser, TokenType type) {
    return peek(parser).type == type;
}

static Token previous(Parser* parser) {
    return parser->tokens[parser->current-1];
}

static bool match(Parser* parser, TokenType type) {
    if (check(parser, type)) {
        advance(parser);
        return true;
    }
    return false;
}

static Token consume(Parser* parser) {
    if (parser->current < parser->count) {
        return parser->tokens[parser->current++];
    }
    return (Token){TOKEN_EOF, 0, 0, 0, 0, NULL};
}






/* CFG Rules */


/* Program Structure */

static ASTNode* parse_program(Parser* parser){}
static ASTNode* parse_top_decl(Parser* parser){}
static ASTNode* parse_import(Parser* parser){}

/* Types */

static Type* parse_type(Parser* parser){}
static Type* parse_builtin_t(Parser* parser){}
static Type* parse_sized_t(Parser* parser){}
static Type* parse_user_t(Parser* parser){}
static Type* parse_pointer_t(Parser* parser, Type* base){}
static Type* parse_array_t(Parser* parser, Type* base){}


/* Declarations */

static ASTNode* parse_func_decl(Parser* parser){}
static ASTNode* parse_var_decl(Parser* parser){}
static ASTNode* parse_struct_decl(Parser* parser){}
static ASTNode* parse_class_decl(Parser* parser){}
static ASTNode* parse_class_member(Parser* parser){}
static ASTNode* parse_struct_member(Parser* parser){}
static ASTNode* parse_param_list(Parser* parser){}
static ASTNode* parse_param(Parser* parser){}


/* Statements */

static ASTNode* parse_statement(Parser* parser){}

static ASTNode* parse_if_stmt(Parser* parser){}
static ASTNode* parse_while_stmt(Parser* parser){}
static ASTNode* parse_for_stmt(Parser* parser){}
static ASTNode* parse_return_stmt(Parser* parser){}
static ASTNode* parse_expression_stmt(Parser* parser){}

static ASTNode* parse_block(Parser* parser){}


/* Expressions */

static ASTNode* parse_expression(Parser* parser){}
static ASTNode* parse_assignment(Parser* parser){}
static ASTNode* parse_logical_or(Parser* parser){}
static ASTNode* parse_logical_and(Parser* parser){}
static ASTNode* parse_equality(Parser* parser){}
static ASTNode* parse_comparison(Parser* parser){}
static ASTNode* parse_additive(Parser* parser){}
static ASTNode* parse_multiplicative(Parser* parser){}
static ASTNode* parse_primary(Parser* parser){}
static ASTNode* parse_unary(Parser* parser){}



/* Main Functions */

Parser* init_parser(Token* tks, uint32_t count, ErrorList* error_list) {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->errors = error_list;
    parser->count = count;


}

ASTNode* run_parser(Parser* parser) {

}

ASTNode* create_ast_node(ASTNode* left, ASTNode* right) {

}

static void synchronize(Parser* parser) {

}
