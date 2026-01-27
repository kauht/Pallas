grammar Pallas;

// -----------------------------
// Parser rules
// -----------------------------

module
    : (importDecl | declaration)* EOF
    ;

importDecl
    : IMPORT importPath ';'
    ;

importPath
    : STRING_LITERAL
    | IDENT ( (DOUBLE_COLON IDENT)
            | (DOT IDENT)
            | (SLASH IDENT)
            )*
    ;

// Top-level declarations
declaration
    : enumDecl
    | structDecl
    | classDecl
    | functionDecl
    | globalVarDecl
    | constDecl
    ;

// Enums
enumDecl
    : ENUM IDENT '{' enumVariant (',' enumVariant)* (',' )? '}'
    ;

enumVariant
    : IDENT ('=' expression)?                                // discriminant assigned
    | IDENT '(' paramList ')'                                // tuple-like data
    | IDENT '(' fieldList ')'                                // named fields variant
    ;

// Structs
structDecl
    : STRUCT IDENT '{' fieldDecl* '}'
    ;

fieldDecl
    : IDENT ':' type ';'
    ;

// Classes
classDecl
    : CLASS IDENT '{' classBody* '}'
    ;

classBody
    : accessSpec ':'                                            // access label (public:, private:, protected:)
    | constructorDecl
    | destructorDecl
    | methodDecl
    | fieldDecl
    ;

accessSpec
    : PUBLIC
    | PRIVATE
    | PROTECTED
    ;

constructorDecl
    : IDENT '(' paramList? ')' block
    ;

destructorDecl
    : '~' IDENT '(' ')' block
    ;

methodDecl
    : IDENT '(' paramList? ')' (':' type)? block
    ;

// Functions (top-level and local)
functionDecl
    : IDENT '(' paramList? ')' (':' type)? block
    ;

// Globals & consts
globalVarDecl
    : IDENT ':' type '=' expression ';'
    ;

constDecl
    : CONST IDENT ':' type '=' expression ';'
    ;

// Types
type
    : simpleType ( typeSuffix )*
    ;

simpleType
    : IDENT
    | primitiveType
    | '(' type ')'
    ;

primitiveType
    : 'int' | 'i8' | 'i16' | 'i32' | 'i64'
    | 'u8' | 'u16' | 'u32' | 'u64'
    | 'f32' | 'f64' | 'bool' | 'string' | 'void'
    ;

typeSuffix
    : '*'                              // pointer
    | '[' INT_LITERAL ']'              // fixed-size array
    | '[' ']'                          // dynamic/unspecified array
    | '<' type (',' type)* '>'         // generics (param list)
    ;

// Parameters
paramList
    : param (',' param)* (',' ELLIPSIS)?
    | ELLIPSIS
    ;

param
    : IDENT ':' type
    ;

// Field list used in enum variant decls
fieldList
    : param (',' param)*
    ;

// Statements and blocks
block
    : '{' statement* '}'
    ;

statement
    : block
    | varDecl
    | expressionStmt
    | ifStmt
    | whileStmt
    | forStmt
    | returnStmt
    | breakStmt
    | continueStmt
    | matchStmt
    | ';'                        // empty statement
    ;

varDecl
    : IDENT ':' type '=' expression ';'
    ;

// Expression statement
expressionStmt
    : expression ';'
    ;

// Control flow
ifStmt
    : IF '(' expression ')' block ( ELSE ( ifStmt | block ) )*
    ;

whileStmt
    : WHILE '(' expression ')' block
    ;

forStmt
    : FOR '(' forInit? ';' expression? ';' forUpdate? ')' block
    | FOR '(' varDeclNoSemicolon ')' block
    | FOR '(' ';' ';' ')' block
    ;

forInit
    : varDeclNoSemicolon
    | expression
    ;

varDeclNoSemicolon
    : IDENT ':' type '=' expression
    ;

forUpdate
    : expression
    | IDENT '=' expression
    ;

// Match / pattern matching
matchStmt
    : MATCH '(' expression ')' '{' matchArm* '}'
    ;

matchArm
    : pattern ( IF expression )? '=>' blockOrExpr
    ;

blockOrExpr
    : block
    | expressionStmt
    ;

// Patterns
pattern
    : '_'
    | IDENT
    | literal
    | IDENT DOUBLE_COLON IDENT ( '(' patternList? ')' )?            // Enum::Variant(...)
    | IDENT '{' patternFieldList? '}'                                // struct patterns
    | patternOr
    ;

patternList
    : pattern (',' pattern)*
    ;

patternFieldList
    : IDENT ':' pattern (',' IDENT ':' pattern)*
    ;

patternOr
    : pattern '|' pattern ( '|' pattern )*
    ;

// Returns
returnStmt
    : RETURN expression? ';'
    ;

breakStmt
    : BREAK ';'
    ;

continueStmt
    : CONTINUE ';'
    ;

// Expressions (operator precedence implemented via rule hierarchy)
// Top level expression entry
expression
    : assignment
    ;

// Assignment (right-associative)
assignment
    : conditional ( assignOp assignment )?
    ;

assignOp
    : '=' | PLUS_ASSIGN | MINUS_ASSIGN | STAR_ASSIGN | SLASH_ASSIGN | PERCENT_ASSIGN
    | AMP_ASSIGN | PIPE_ASSIGN | XOR_ASSIGN | LSHIFT_ASSIGN | RSHIFT_ASSIGN
    ;

// Conditional (ternary)
conditional
    : logical_or ( '?' expression ':' conditional )?
    ;

// Logical OR/AND
logical_or
    : logical_and ( '||' logical_and )*
    ;

logical_and
    : bitwise_or ( '&&' bitwise_or )*
    ;

// Bitwise ops
bitwise_or
    : bitwise_xor ( '|' bitwise_xor )*
    ;

bitwise_xor
    : bitwise_and ( '^' bitwise_and )*
    ;

bitwise_and
    : equality ( '&' equality )*
    ;

// Equality and relational
equality
    : relational ( ( '==' | '!=' ) relational )*
    ;

relational
    : shift ( ( '<' | '<=' | '>' | '>=' ) shift )*
    ;

// Shift
shift
    : additive ( ( '<<' | '>>' ) additive )*
    ;

// Additive and multiplicative
additive
    : multiplicative ( ( '+' | '-' ) multiplicative )*
    ;

multiplicative
    : unary ( ( '*' | '/' | '%' ) unary )*
    ;

// Unary (prefix)
unary
    : ( '!' | '~' | '+' | '-' | '++' | '--' | '*' | '&' ) unary
    | postfix
    ;

// Postfix (call, index, member access, postfix ++/--)
postfix
    : primary ( postfixOp )*
    ;

postfixOp
    : '(' argList? ')'
    | '[' expression ']'
    | '.' IDENT
    | '->' IDENT
    | '++'
    | '--'
    ;

// Function arguments
argList
    : expression (',' expression)*
    ;

// Primary expressions
primary
    : literal
    | IDENT
    | '(' expression ')'
    | structInit
    | arrayInit
    | lambdaExpr
    ;

// Struct init: Type { field: expr, ... }
structInit
    : IDENT '{' fieldInitList? '}'
    ;

fieldInitList
    : IDENT ':' expression (',' IDENT ':' expression)*
    ;

// Array init: [ expr, ... ]
arrayInit
    : '[' expressionList? ']'
    ;

expressionList
    : expression (',' expression)*
    ;

// Lambda (placeholder - basic form)
lambdaExpr
    : '(' paramList? ')' '=>' expression
    ;

// Literals
literal
    : INT_LITERAL
    | FLOAT_LITERAL
    | CHAR_LITERAL
    | STRING_LITERAL
    | BOOL_LITERAL
    | NULL_LITERAL
    ;

// -----------------------------
// Lexer rules
// -----------------------------

IMPORT      : 'import' ;
ENUM        : 'enum' ;
STRUCT      : 'struct' ;
CLASS       : 'class' ;
PUBLIC      : 'public' ;
PRIVATE     : 'private' ;
PROTECTED   : 'protected' ;
CONST       : 'const' ;
RETURN      : 'return' ;
MATCH       : 'match' ;
IF          : 'if' ;
ELSE        : 'else' ;
WHILE       : 'while' ;
FOR         : 'for' ;
BREAK       : 'break' ;
CONTINUE    : 'continue' ;
NEW         : 'new' ;
DELETE      : 'delete' ;
TRUE        : 'true' ;
FALSE       : 'false' ;
NULL        : 'null' ;

// Operators & punctuation
ELLIPSIS    : '...' ;
DOUBLE_COLON: '::' ;
ARROW       : '->' ;
PLUS_ASSIGN : '+=' ;
MINUS_ASSIGN: '-=' ;
STAR_ASSIGN : '*=' ;
SLASH_ASSIGN: '/=' ;
PERCENT_ASSIGN: '%=' ;
AMP_ASSIGN  : '&=' ;
PIPE_ASSIGN : '|=' ;
XOR_ASSIGN  : '^=' ;
LSHIFT_ASSIGN: '<<=' ;
RSHIFT_ASSIGN: '>>=' ;

PLUS_PLUS   : '++' ;
MINUS_MINUS : '--' ;

// Single/double char tokens (explicit for clarity)
LT_EQ       : '<=' ;
GT_EQ       : '>=' ;
EQ_EQ       : '==' ;
NOT_EQ      : '!=' ;
LSHIFT     : '<<' ;
RSHIFT     : '>>' ;
LOGICAL_AND : '&&' ;
LOGICAL_OR  : '||' ;

SEMICOLON   : ';' ;
COMMA       : ',' ;
COLON       : ':' ;
DOT         : '.' ;
LPAREN      : '(' ;
RPAREN      : ')' ;
LBRACE      : '{' ;
RBRACE      : '}' ;
LBRACKET    : '[' ;
RBRACKET    : ']' ;
QUESTION    : '?' ;
AT          : '@' ;
TILDE       : '~' ;
CARET       : '^' ;
AMP         : '&' ;
PIPE        : '|' ;
PLUS        : '+' ;
MINUS       : '-' ;
STAR        : '*' ;
SLASH       : '/' ;
PERCENT     : '%' ;
ASSIGN      : '=' ;
LT          : '<' ;
GT          : '>' ;
BANG        : '!' ;

// Literals
INT_LITERAL
    : DIGIT+
    ;

FLOAT_LITERAL
    : DIGIT+ '.' DIGIT* EXP?    // 1. or 1.0 or 1.0e+5
    | DIGIT+ EXP                 // 1e10
    ;

CHAR_LITERAL
    : '\'' ( ESC_SEQ | ~('\''|'\\') ) '\''
    ;

STRING_LITERAL
    : '"' ( ESC_SEQ | ~('"'|'\\') )* '"'
    ;

// Booleans and null (also covered by keywords)
BOOL_LITERAL
    : TRUE | FALSE
    ;

NULL_LITERAL
    : NULL
    ;

// Identifiers (keywords are matched first due to lexer rule order)
IDENT
    : [A-Za-z_][A-Za-z0-9_]*
    ;

// Comments & whitespace
LINE_COMMENT
    : '//' ~[\r\n]* -> skip
    ;

BLOCK_COMMENT
    : '/*' .*? '*/' -> skip
    ;

WS
    : [ \t\r\n]+ -> skip
    ;

// Fragments
fragment DIGIT
    : [0-9]
    ;

fragment EXP
    : [eE] [+\-]? DIGIT+
    ;

fragment ESC_SEQ
    : '\\' [btnr"'\\]        // common escapes
    | '\\' 'x' HEX HEX       // hex
    | '\\' [0-7]{1,3}        // octal
    ;

fragment HEX
    : [0-9a-fA-F]
    ;

// -----------------------------
// End of grammar
// -----------------------------
