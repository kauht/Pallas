PROGRAM            ::= { TOP_LEVEL_DECL } ;

TOP_LEVEL_DECL     ::= IMPORT
                     | FUNCTION_DECL
                     | STRUCT_DECL
                     | CLASS_DECL
                     | TYPE_ALIAS
                     | CONST_DECL
                     | GLOBAL_DECL ;

IMPORT             ::= "import" STRING ";" ;
GLOBAL_DECL        ::= DECLARATION ;
CONST_DECL         ::= "const" IDENTIFIER ":" TYPE "=" CONST_EXPR ";" ;

DECLARATION        ::= IDENTIFIER [ ":" TYPE ] [ "=" EXPRESSION ] ";" ;
FUNCTION_DECL      ::= IDENTIFIER [ TYPE_PARAMS ] "(" [ PARAM_LIST ] ")" [ RETURN_TYPE ] BLOCK ;
STRUCT_DECL        ::= "struct" IDENTIFIER [ TYPE_PARAMS ] STRUCT_BODY ;
CLASS_DECL         ::= "class" IDENTIFIER [ TYPE_PARAMS ] CLASS_BODY ;
TYPE_ALIAS         ::= "type" IDENTIFIER [ TYPE_PARAMS ] "=" TYPE ";" ;

TYPE_PARAMS        ::= "<" TYPE_PARAM { "," TYPE_PARAM } ">" ;
TYPE_PARAM         ::= IDENTIFIER [ "=" TYPE ] ;
TYPE_ARGS          ::= "<" TYPE { "," TYPE } ">" ;

TYPE               ::= TYPE_ATOM [ TYPE_ARGS ]
                     | POINTER_TYPE
                     | REFERENCE_TYPE
                     | ARRAY_TYPE
                     | FUNCTION_TYPE ;

TYPE_ATOM          ::= PRIMITIVE_TYPE | INTEGER_WIDTH_TYPE | IDENTIFIER ;
INTEGER_WIDTH_TYPE ::= ( "i" | "u" ) DIGITS ;
PRIMITIVE_TYPE     ::= "f32" | "f64" | "bool" | "char" | "string" | "void" ;

POINTER_TYPE       ::= TYPE "*" ;
REFERENCE_TYPE     ::= TYPE "&" ;
ARRAY_TYPE         ::= TYPE "[" EXPRESSION "]" ;
FUNCTION_TYPE      ::= "(" [ PARAM_LIST ] ")" RETURN_TYPE ;

PARAM_LIST         ::= PARAM { "," PARAM } ;
PARAM              ::= IDENTIFIER ":" TYPE ;
RETURN_TYPE        ::= ":" TYPE ;

STRUCT_BODY        ::= "{" { STRUCT_FIELD } "}" ;
STRUCT_FIELD       ::= IDENTIFIER ":" TYPE ";" ;

CLASS_BODY         ::= "{" { CLASS_MEMBER } "}" ;
CLASS_MEMBER       ::= VISIBILITY_SECTION
                     | FIELD_DECL
                     | METHOD_DECL
                     | CONSTRUCTOR_DECL
                     | DESTRUCTOR_DECL ;
VISIBILITY_SECTION ::= ( "public" | "private" ) BLOCK ;
FIELD_DECL         ::= IDENTIFIER ":" TYPE ";" ;
METHOD_DECL        ::= IDENTIFIER [ TYPE_PARAMS ] "(" [ PARAM_LIST ] ")" [ RETURN_TYPE ] BLOCK ;
CONSTRUCTOR_DECL   ::= IDENTIFIER "(" [ PARAM_LIST ] ")" BLOCK ;
DESTRUCTOR_DECL    ::= "~" IDENTIFIER "(" ")" BLOCK ;

BLOCK              ::= "{" { STATEMENT } "}" ;
STATEMENT          ::= DECLARATION
                     | IF_STATEMENT
                     | WHILE_STATEMENT
                     | FOR_STATEMENT
                     | MATCH_STATEMENT
                     | RETURN_STATEMENT
                     | EXPRESSION_STATEMENT
                     | ARENA_BLOCK
                     | BLOCK ;

EXPRESSION_STATEMENT ::= EXPRESSION ";" ;
IF_STATEMENT       ::= "if" "(" EXPRESSION ")" STATEMENT [ "else" STATEMENT ] ;
WHILE_STATEMENT    ::= "while" "(" EXPRESSION ")" STATEMENT ;

FOR_STATEMENT      ::= "for" "(" ( DECLARATION | EXPRESSION )? ";" EXPRESSION? ";" EXPRESSION? ")" STATEMENT
                     | "for" "(" IDENTIFIER ":" EXPRESSION ")" STATEMENT ;

ARENA_BLOCK        ::= "arena" "(" [ EXPRESSION ] ")" BLOCK ;

MATCH_STATEMENT    ::= "match" "(" EXPRESSION ")" "{" { MATCH_ARM } "}" ;
MATCH_ARM          ::= PATTERN "=>" BLOCK ;

RETURN_STATEMENT   ::= "return" [ EXPRESSION ] ";" ;

PATTERN            ::= LITERAL_PATTERN | WILDCARD_PATTERN | IDENTIFIER_PATTERN ;
LITERAL_PATTERN    ::= LITERAL ;
WILDCARD_PATTERN   ::= "_" ;
IDENTIFIER_PATTERN ::= IDENTIFIER ;

EXPRESSION         ::= ASSIGNMENT_EXPR ;
ASSIGNMENT_EXPR    ::= IDENTIFIER "=" ASSIGNMENT_EXPR
                     | LOGICAL_OR_EXPR ;
LOGICAL_OR_EXPR    ::= LOGICAL_OR_EXPR "||" LOGICAL_AND_EXPR | LOGICAL_AND_EXPR ;
LOGICAL_AND_EXPR   ::= LOGICAL_AND_EXPR "&&" EQUALITY_EXPR | EQUALITY_EXPR ;
EQUALITY_EXPR      ::= EQUALITY_EXPR "==" COMPARISON_EXPR
                     | EQUALITY_EXPR "!=" COMPARISON_EXPR
                     | COMPARISON_EXPR ;
COMPARISON_EXPR    ::= COMPARISON_EXPR "<" ADDITIVE_EXPR
                     | COMPARISON_EXPR ">" ADDITIVE_EXPR
                     | COMPARISON_EXPR "<=" ADDITIVE_EXPR
                     | COMPARISON_EXPR ">=" ADDITIVE_EXPR
                     | ADDITIVE_EXPR ;
ADDITIVE_EXPR      ::= ADDITIVE_EXPR "+" MULTIPLICATIVE_EXPR
                     | ADDITIVE_EXPR "-" MULTIPLICATIVE_EXPR
                     | MULTIPLICATIVE_EXPR ;
MULTIPLICATIVE_EXPR ::= MULTIPLICATIVE_EXPR "*" UNARY_EXPR
                     | MULTIPLICATIVE_EXPR "/" UNARY_EXPR
                     | MULTIPLICATIVE_EXPR "%" UNARY_EXPR
                     | UNARY_EXPR ;
UNARY_EXPR         ::= "!" UNARY_EXPR | "-" UNARY_EXPR | POSTFIX_EXPR ;
POSTFIX_EXPR       ::= PRIMARY_EXPR | CALL_EXPR | INDEX_EXPR | MEMBER_ACCESS_EXPR ;

PRIMARY_EXPR       ::= LITERAL | IDENTIFIER | "(" EXPRESSION ")" | NEW_EXPR | DELETE_EXPR ;
CALL_EXPR          ::= PRIMARY_EXPR [ TYPE_ARGS ] "(" [ ARGUMENT_LIST ] ")" ;
ARGUMENT_LIST      ::= EXPRESSION { "," EXPRESSION } ;
INDEX_EXPR         ::= PRIMARY_EXPR "[" EXPRESSION "]" ;
MEMBER_ACCESS_EXPR ::= PRIMARY_EXPR "." IDENTIFIER [ TYPE_ARGS ] "(" [ ARGUMENT_LIST ] ")"
                     | PRIMARY_EXPR "." IDENTIFIER ;

NEW_EXPR           ::= "new" TYPE | "new" "(" IDENTIFIER ")" TYPE ;
DELETE_EXPR        ::= "delete" EXPRESSION ;

LITERAL            ::= INTEGER_LITERAL | FLOAT_LITERAL | STRING_LITERAL | CHAR_LITERAL | BOOLEAN_LITERAL ;
INTEGER_LITERAL    ::= NUMBER ;
FLOAT_LITERAL      ::= FLOAT ;
STRING_LITERAL     ::= STRING ;
CHAR_LITERAL       ::= CHAR ;
BOOLEAN_LITERAL    ::= "true" | "false" ;

CONST_EXPR         ::= LITERAL | IDENTIFIER | CONST_EXPR_BINOP ;
CONST_EXPR_BINOP   ::= CONST_EXPR ( "+" | "-" | "*" | "/" | "%" ) CONST_EXPR ;

DIGITS             ::= /[0-9]+/ ;
IDENTIFIER         ::= /[a-zA-Z_][a-zA-Z0-9_]*/ ;
NUMBER             ::= /[0-9]+/ ;
FLOAT              ::= /[0-9]+\.[0-9]+/ ;
STRING             ::= /"([^"\\]|\\.)*"/ ;
CHAR               ::= /'([^'\\]|\\.)'/ ;