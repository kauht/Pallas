# =========================
# LEXICAL
# =========================

IDENTIFIER      -> [a-zA-Z_][a-zA-Z0-9_]*
NUMBER          -> [0-9]+
FLOAT           -> [0-9]+\.[0-9]+
STRING          -> "\"([^\"\\]|\\.)*\""
CHAR            -> "'([^'\\]|\\.)'"
BOOLEAN_LITERAL -> "true" | "false"

# =========================
# PROGRAM STRUCTURE
# =========================

PROGRAM         -> TOP_LEVEL_DECL*

TOP_LEVEL_DECL  -> IMPORT
                 | FUNCTION_DECL
                 | STRUCT_DECL
                 | CLASS_DECL
                 | GLOBAL_DECL

IMPORT          -> "import" STRING ";"
GLOBAL_DECL     -> DECLARATION

# =========================
# DECLARATIONS
# =========================

DECLARATION     -> IDENTIFIER (":" TYPE)? ("=" EXPRESSION)? ";"
FUNCTION_DECL   -> IDENTIFIER "(" PARAM_LIST? ")" RETURN_TYPE? BLOCK
STRUCT_DECL     -> "struct" IDENTIFIER STRUCT_BODY
CLASS_DECL      -> "class" IDENTIFIER CLASS_BODY

# =========================
# TYPES
# =========================

TYPE            -> TYPE_NAME
                 | POINTER_TYPE
                 | ARRAY_TYPE
                 | FUNCTION_TYPE

TYPE_NAME       -> PRIMITIVE_TYPE | IDENTIFIER
PRIMITIVE_TYPE  -> "i8" | "i16" | "i32" | "i64" | "i128"
                 | "u8" | "u16" | "u32" | "u64" | "u128"
                 | "f32" | "f64"
                 | "bool"
                 | "char"
                 | "string"
                 | "void"

POINTER_TYPE    -> TYPE "*"
ARRAY_TYPE      -> TYPE "[" EXPRESSION "]"
FUNCTION_TYPE   -> "(" PARAM_LIST? ")" RETURN_TYPE

# =========================
# PARAMETERS
# =========================

PARAM_LIST      -> PARAM ("," PARAM)*
PARAM           -> IDENTIFIER ":" TYPE
RETURN_TYPE     -> ":" TYPE

# =========================
# STRUCTS
# =========================

STRUCT_BODY     -> "{" STRUCT_FIELD* "}"
STRUCT_FIELD    -> IDENTIFIER ":" TYPE ";"

# =========================
# CLASSES
# =========================

CLASS_BODY      -> "{" CLASS_MEMBER* "}"
CLASS_MEMBER    -> VISIBILITY_SECTION
                 | FIELD_DECL
                 | METHOD_DECL
                 | CONSTRUCTOR_DECL
                 | DESTRUCTOR_DECL

VISIBILITY_SECTION -> ("public" | "private") BLOCK

FIELD_DECL      -> IDENTIFIER ":" TYPE ";"
METHOD_DECL     -> IDENTIFIER "(" PARAM_LIST? ")" RETURN_TYPE? BLOCK
CONSTRUCTOR_DECL -> IDENTIFIER "(" PARAM_LIST? ")" BLOCK
DESTRUCTOR_DECL -> "~" IDENTIFIER "(" ")" BLOCK

# =========================
# STATEMENTS
# =========================

STATEMENT       -> DECLARATION
                 | IF_STATEMENT
                 | WHILE_STATEMENT
                 | FOR_STATEMENT
                 | MATCH_STATEMENT
                 | RETURN_STATEMENT
                 | EXPRESSION_STATEMENT
                 | BLOCK

BLOCK           -> "{" STATEMENT* "}"

EXPRESSION_STATEMENT -> EXPRESSION ";"

IF_STATEMENT    -> "if" "(" EXPRESSION ")" STATEMENT ("else" STATEMENT)?
WHILE_STATEMENT -> "while" "(" EXPRESSION ")" STATEMENT
FOR_STATEMENT   -> "for" "(" (DECLARATION | EXPRESSION)? ";" EXPRESSION? ";" EXPRESSION? ")" STATEMENT
MATCH_STATEMENT -> "match" "(" EXPRESSION ")" "{" MATCH_ARM* "}"
MATCH_ARM       -> PATTERN "=>" BLOCK
RETURN_STATEMENT -> "return" EXPRESSION? ";"

# =========================
# PATTERNS
# =========================

PATTERN         -> LITERAL_PATTERN | WILDCARD_PATTERN
LITERAL_PATTERN -> LITERAL
WILDCARD_PATTERN -> "_"

# =========================
# EXPRESSIONS (FULL PRECEDENCE)
# =========================

EXPRESSION          -> ASSIGNMENT_EXPR

ASSIGNMENT_EXPR     -> IDENTIFIER "=" ASSIGNMENT_EXPR
                     | LOGICAL_OR_EXPR

LOGICAL_OR_EXPR     -> LOGICAL_OR_EXPR "||" LOGICAL_AND_EXPR
                     | LOGICAL_AND_EXPR

LOGICAL_AND_EXPR    -> LOGICAL_AND_EXPR "&&" EQUALITY_EXPR
                     | EQUALITY_EXPR

EQUALITY_EXPR       -> EQUALITY_EXPR "==" COMPARISON_EXPR
                     | EQUALITY_EXPR "!=" COMPARISON_EXPR
                     | COMPARISON_EXPR

COMPARISON_EXPR     -> COMPARISON_EXPR "<" ADDITIVE_EXPR
                     | COMPARISON_EXPR ">" ADDITIVE_EXPR
                     | COMPARISON_EXPR "<=" ADDITIVE_EXPR
                     | COMPARISON_EXPR ">=" ADDITIVE_EXPR
                     | ADDITIVE_EXPR

ADDITIVE_EXPR       -> ADDITIVE_EXPR "+" MULTIPLICATIVE_EXPR
                     | ADDITIVE_EXPR "-" MULTIPLICATIVE_EXPR
                     | MULTIPLICATIVE_EXPR

MULTIPLICATIVE_EXPR -> MULTIPLICATIVE_EXPR "*" UNARY_EXPR
                     | MULTIPLICATIVE_EXPR "/" UNARY_EXPR
                     | MULTIPLICATIVE_EXPR "%" UNARY_EXPR
                     | UNARY_EXPR

UNARY_EXPR          -> "!" UNARY_EXPR
                     | "-" UNARY_EXPR
                     | POSTFIX_EXPR

POSTFIX_EXPR        -> PRIMARY_EXPR
                     | CALL_EXPR
                     | INDEX_EXPR
                     | MEMBER_ACCESS_EXPR

PRIMARY_EXPR        -> LITERAL
                     | IDENTIFIER
                     | "(" EXPRESSION ")"
                     | NEW_EXPR
                     | DELETE_EXPR

CALL_EXPR           -> PRIMARY_EXPR "(" ARGUMENT_LIST? ")"
ARGUMENT_LIST       -> EXPRESSION ("," EXPRESSION)*

INDEX_EXPR          -> PRIMARY_EXPR "[" EXPRESSION "]"
MEMBER_ACCESS_EXPR  -> PRIMARY_EXPR "." IDENTIFIER

NEW_EXPR            -> "new" TYPE
DELETE_EXPR         -> "delete" EXPRESSION

# =========================
# LITERALS
# =========================

LITERAL             -> INTEGER_LITERAL
                     | FLOAT_LITERAL
                     | STRING_LITERAL
                     | CHAR_LITERAL
                     | BOOLEAN_LITERAL

INTEGER_LITERAL     -> NUMBER
FLOAT_LITERAL       -> FLOAT
STRING_LITERAL      -> STRING
CHAR_LITERAL        -> CHAR
BOOLEAN_LITERAL     -> BOOLEAN_LITERAL
