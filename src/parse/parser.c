#include "parser.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "diagnostic.h"
#include "platform.h"

/* Helper */

static bool at_end(Parser* parser) {
    return parser->current >= parser->count || parser->tokens[parser->current].type == TOKEN_EOF;
}

static Token advance(Parser* parser) {
    if (!at_end(parser)) {
        parser->current++;
    }
    return parser->tokens[parser->current - 1];
}

static Token peek(Parser* parser) {
    return parser->tokens[parser->current];
}

static bool check(Parser* parser, TokenType type) {
    return peek(parser).type == type;
}

static Token previous(Parser* parser) {
    if (parser->current == 0) {
        return (Token){NULL, 0, 0, 0, 0, TOKEN_EOF};
    }
    return parser->tokens[parser->current - 1];
}

static bool _match(Parser* parser, TokenType type) {
    if (check(parser, type)) {
        advance(parser);
        return true;
    }
    return false;
}
static Token consume(Parser* parser) {
    if (!at_end(parser)) {
        return parser->tokens[parser->current++];
    }
    return (Token){NULL, 0, 0, 0, 0, TOKEN_EOF};
}

static bool match(Parser* parser, uint32_t count, ...) {
    va_list args;
    va_start(args, count);

    for (uint32_t i = 0; i < count; i++) {
        TokenType tk = va_arg(args, TokenType);
        if (check(parser, tk)) {
            advance(parser);
            va_end(args);
            return true;
        }
    }

    va_end(args);
    return false;
}

/* CFG Rules */

static void free_ast_node(ASTNode* node) {
    if (!node) return;
    free(node);
}


static ASTNode* parse_top_decl(Parser* parser) {
    return NULL;
}

static ASTNode* parse_import(Parser* parser) {
    ASTNode* node = create_ast_node(AST_IMPORT, peek(parser).line, peek(parser).column);
    if (!node) return NULL;
    if (advance(parser).type != TOKEN_IMPORT) {
        free(node);
        return NULL;
    }

    return NULL;
}

/* Types */

static Type* parse_type(Parser* parser) {
    return NULL;
    // Built-in | User | Pointer
}
static Type* parse_builtin_t(Parser* parser) {
    // TOKEN_INT | TOKEN_FLOAT | TOKEN_DOUBLE | TOKEN_CHAR | TOKEN_STRING | TOKEN_BOOL
    return NULL;
}
static Type* parse_sized_t(Parser* parser) {
    // | TOKEN_I8 | TOKEN_I16 | TOKEN_I32 | TOKEN_I64
    // | TOKEN_U8 | TOKEN_U16 | TOKEN_U32 | TOKEN_U64
    // | TOKEN_F32 | TOKEN_F64
    return NULL;
}
static Type* parse_user_t(Parser* parser) {
    // Custom Class/Structs
    return NULL;
}
static Type* parse_pointer_t(Parser* parser, Type* base) {
    return NULL;
}
static Type* parse_array_t(Parser* parser, Type* base) {
    return NULL;
}

/* Declarations */

static ASTNode* parse_func_decl(Parser* parser) {
    return NULL;
}
static ASTNode* parse_var_decl(Parser* parser) {
    return NULL;
}
static ASTNode* parse_struct_decl(Parser* parser) {
    return NULL;
}
static ASTNode* parse_class_decl(Parser* parser) {
    return NULL;
}
static ASTNode* parse_class_member(Parser* parser) {
    return NULL;
}
static ASTNode* parse_struct_member(Parser* parser) {
    return NULL;
}
static ASTNode* parse_param_list(Parser* parser) {
    return NULL;
}
static ASTNode* parse_param(Parser* parser) {
    return NULL;
}

/* Statements */

static ASTNode* parse_statement(Parser* parser) {
    return NULL;
}

static ASTNode* parse_if_stmt(Parser* parser) {
    return NULL;
}
static ASTNode* parse_while_stmt(Parser* parser) {
    return NULL;
}
static ASTNode* parse_for_stmt(Parser* parser) {
    return NULL;
}
static ASTNode* parse_return_stmt(Parser* parser) {
    return NULL;
}
static ASTNode* parse_expression_stmt(Parser* parser) {
    return NULL;
}

static ASTNode* parse_block(Parser* parser) {
    return NULL;
}

/* Expressions */

static ASTNode* parse_expression(Parser* parser) {
    return NULL;
}
static ASTNode* parse_assignment(Parser* parser) {
    return NULL;
}
static ASTNode* parse_logical_or(Parser* parser) {
    return NULL;
}
static ASTNode* parse_logical_and(Parser* parser) {
    return NULL;
}
static ASTNode* parse_equality(Parser* parser) {
    return NULL;
}
static ASTNode* parse_comparison(Parser* parser) {
    return NULL;
}
static ASTNode* parse_additive(Parser* parser) {
    return NULL;
}
static ASTNode* parse_multiplicative(Parser* parser) {
    return NULL;
}
static ASTNode* parse_primary(Parser* parser) {
    TokenType t = peek(parser).type;
    if (t == TOKEN_IDENT) {
        // Create Identifier Node
    }
    if (t == TOKEN_INT_LITERAL || t == TOKEN_CHAR_LITERAL || t == TOKEN_FLOAT_LITERAL ||
        t == TOKEN_STRING_LITERAL) {
    }
    if (t == TOKEN_LPAREN) {
        consume(parser);
        parse_expression(parser);
        if (peek(parser).type != TOKEN_RPAREN) {
            errors_add(parser->errors, NULL, "Expected Expression", SEVERITY_ERROR,
                       parser->tokens[parser->current].line, parser->tokens[parser->current].column,
                       0, CATEGORY_PARSER);
            synchronize(parser);
            return create_ast_node(AST_UNKNOWN, parser->tokens[parser->current].line,
                                   parser->tokens[parser->current].column);
        }
    }
    return NULL;
}
static ASTNode* parse_unary(Parser* parser) {
    return NULL;
}

static ASTNode* parse_program(Parser* parser) {
    ASTNode* program = create_ast_node(AST_PROGRAM, peek(parser).line, peek(parser).column);
    if (!program) {
        return NULL;
    }

    program->program.imports = NULL;
    program->program.import_count = 0;
    program->program.import_capacity = 0;
    program->program.top_decls = NULL;
    program->program.top_decl_count = 0;
    program->program.top_decl_capacity = 0;

    while (!at_end(parser) && check(parser, TOKEN_IMPORT)) {
        ASTNode* import_node = parse_import(parser);
        if (!import_node) {
            if (at_end(parser)) break;
            synchronize(parser);
            continue;
        }
    }

    while (!at_end(parser)) {
        ASTNode* decl = parse_top_decl(parser);
        if (!decl) {
            if (at_end(parser)) {
                break;
            }
            synchronize(parser);
            continue;
        }
    }

    return program;
}

/* Main Functions */

Parser* init_parser(const char* filename, Token* tks, uint32_t count, ErrorList* error_list) {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    if (!parser) {
        return NULL;
    }
    parser->filename = filename;
    parser->errors = error_list;
    parser->count = count;
    parser->panic = false;
    parser->tokens = tks;
    parser->current = 0;
    return parser;
}

ASTNode* run_parser(Parser* parser) {
    if (!parser) return NULL;
    return parse_program(parser);
}

ASTNode* create_ast_node(ASTNodeType type, uint32_t line, uint32_t column) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node)
        return NULL;

    node->position.line = line;
    node->position.column = column;
    node->position.length = 0;
    node->start = 0;
    node->type = type;
    return node;
}

void synchronize(Parser* parser) {
    while (!at_end(parser)) {
        if (match(parser, 8, TOKEN_SEMICOLON, TOKEN_RBRACE, TOKEN_COMMA, TOKEN_IMPORT, TOKEN_FOR,
                  TOKEN_IF, TOKEN_WHILE, TOKEN_RETURN)) {
            parser->panic = false;
            return;
        }

        consume(parser);
    }
    parser->panic = false;
}
