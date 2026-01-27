#include "lexer.h"
#include <cctype>
#include <iostream>


bool Lexer::at_end() {
    return position >= file.content.length();
}
char Lexer::next_char() {
    char c = peek_char();
    if (at_end()) return '\0';
    if (c == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }
    return file.content.at(position++);
}
char Lexer::peek_char() {
    if (position >= file.content.length()) return '\0';
    return file.content.at(position);
}
Token Lexer::create_token(const std::string& lexeme, uint32_t start_line, uint32_t start_column, TokenType type) {
    return Token{lexeme, start_line, start_column, type};
}
TokenType Lexer::is_keyword(const std::string& s) {
    auto it = keywords.find(s);
    if (it != keywords.end()) return it->second;
    return TOKEN_IDENT;
}
void Lexer::skip_untracked() {
    while (!at_end()) {
        char c = peek_char();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            next_char();
            continue;
        }
        if (c == '/' && position + 1 < file.content.length() && file.content.at(position+1) == '/') {
            next_char();
            next_char();
            while (peek_char() != '\n' && !at_end()) {
                next_char();
            }
            continue;
        }
        if (c == '/' && position + 1 < file.content.length() && file.content.at(position+1) == '*') {
            next_char();
            next_char();
            while (!at_end()) {
                if (peek_char() == '*' && position + 1 < file.content.length() && file.content.at(position+1) == '/') {
                    break;
                }
                next_char();
            }
            if (!at_end()) {
                next_char();
                next_char();
            } else {
                // Error unterminated block comment
            }
            continue;
        }
        break;
    }
}

// Lexer
Token Lexer::lex_identifier() {
    uint32_t start = position;
    uint32_t start_line = line;
    uint32_t start_column = column;
    while (!at_end() && (isalnum(file.content.at(position)) || file.content.at(position) == '_')) {
        next_char();
    }
    std::string lexeme = file.content.substr(start, position - start);
    TokenType type = is_keyword(lexeme);
    return create_token(lexeme, start_line, start_column, type);
}
Token Lexer::lex_number() {
    uint32_t start = position;
    uint32_t start_line = line;
    uint32_t start_column = column;
    bool is_float = false;
    while (!at_end()) {
        char c = peek_char();
        if (isdigit(c)) {
            next_char();
        } else if (c == '.') {
            if (is_float) {
                // error too many decimal points
                break;
            }
            if (position + 1 < file.content.length() && isdigit(file.content.at(position+1))) {
                is_float = true;
                next_char();
            } else {
                // error trailing decimal
                break;
            }
        } else {
            break;
        }
    }
    std::string lexeme = file.content.substr(start, position - start);
    return create_token(lexeme, start_line, start_column, is_float ? TOKEN_FLOAT_LITERAL : TOKEN_INT_LITERAL);
}
Token Lexer::lex_char() {
    uint32_t start = position;
    uint32_t start_line = line;
    uint32_t start_column = column;
    if (peek_char() == '\'') {
        next_char();
    }
    char c = '\0';
    if (peek_char() == '\\') {
        next_char();
        char esc = peek_char();
        switch (esc) {
            case '\\':
                c = '\\';
                break;
            case '\'':
                c = '\'';
                break;
            case '"':
                c = '"';
                break;
            case 'n':
                c = '\n';
                break;
            case 't':
                c = '\t';
                break;
            case 'r':
                c = '\r';
                break;
            case '0':
                c = '\0';
                break;
            default:
                c = esc;
                break;
        }
        if (!at_end()) {
            next_char();
        }
        } else if (peek_char() != '\'' && peek_char() != '\0') {
        c = next_char();
    }

    if (peek_char() == '\'') {
        next_char();
    } else {
        // error unterminated char literal or too many characters
    }

    std::string lexeme = file.content.substr(start, position - start);
    return create_token(lexeme, start_line, start_column, TOKEN_CHAR_LITERAL);
}
Token Lexer::lex_string() {
    uint32_t start = position;
    uint32_t start_line = line;
    uint32_t start_column = column;

    if (peek_char() == '"') {
        next_char();
    }

    while (!at_end()) {
        char c = peek_char();
        if (c == '"') {
            break;
        }
        if (c == '\\') {
            next_char();
            if (!at_end()){
                next_char();
            }
            continue;
        }
        next_char();
    }

    if (peek_char() == '"') {
        next_char();
    } else {
        // error unterminated string literal
    }

    std::string lexeme = file.content.substr(start, position - start);
    return create_token(lexeme, start_line, start_column, TOKEN_STRING_LITERAL);
}
Token Lexer::lex_operator() {
    uint32_t start = position;
    uint32_t start_line = line;
    uint32_t start_column = column;
    std::string lexeme;
    TokenType type = {};

    char c = next_char();

    switch(c) {
        case '(': {
            type = TOKEN_LPAREN;
            break;
        }
        case ')': {
            type = TOKEN_RPAREN;
            break;
        }
        case '{': {
            type = TOKEN_LBRACE;
            break;
        }
        case '}': {
            type = TOKEN_RBRACE;
            break;
        }
        case '[': {
            type = TOKEN_LBRACKET;
            break;
        }
        case ']': {
            type = TOKEN_RBRACKET;
            break;
        }
        case ';': {
            type = TOKEN_SEMICOLON;
            break;
        }
        case '.': {
            // first '.' already consumed; check the next two chars for '..'
            if (peek_char() == '.' && position + 1 < file.content.length() && file.content.at(position+1) == '.') {
                next_char(); // consume second '.'
                next_char();
                type = TOKEN_ELLIPSIS;
                break;
            }
            type = TOKEN_DOT;
            break;
        }
        case ',': {
            type = TOKEN_COMMA;
            break;
        }
        case ':': {
            if (peek_char() == ':') {
                next_char();
                type = TOKEN_DOUBLE_COLON;
                break;
            }
            type = TOKEN_COLON;
            break;
        }
        case '?': {
            type = TOKEN_QUESTION;
            break;
        }
        case '@': {
            type = TOKEN_AT;
            break;
        }
        case '~': {
            type = TOKEN_TILDE;
            break;
        }
        case '^': {
            type = TOKEN_CARET;
            break;
        }
        case '+': {
            if (peek_char() == '+') {
                next_char();
                type = TOKEN_PLUS_PLUS;
                break;
            }
            if (peek_char() == '=') {
                next_char();
                type = TOKEN_PLUS_ASSIGN;
                break;
            }
            type = TOKEN_PLUS;
            break;
        }
        case '-': {
            if (peek_char() == '-') {
                next_char();
                type = TOKEN_MINUS_MINUS;
                break;
            }
            if (peek_char() == '=') {
                next_char();
                type = TOKEN_MINUS_ASSIGN;
                break;
            }
            if (peek_char() == '>') {
                next_char();
                type = TOKEN_ARROW;
                break;
            }
            type = TOKEN_MINUS;
            break;
        }
        case '*': {
            if (peek_char() == '=') {
                next_char();
                type = TOKEN_STAR_ASSIGN;
                break;
            }
            type = TOKEN_STAR;
            break;
        }
        case '/': {
            if (peek_char() == '=') {
                next_char();
                type = TOKEN_SLASH_ASSIGN;
                break;
            }
            type = TOKEN_SLASH;
            break;
        }
        case '%': {
            type = TOKEN_PERCENT;
            break;
        }
        case '=': {
            if (peek_char() == '=') {
                next_char();
                type = TOKEN_EQUAL;
                break;
            }
            type = TOKEN_ASSIGN;
            break;
        }
        case '!': {
            if (peek_char() == '=') {
                next_char();
                type = TOKEN_NOT_EQUAL;
                break;
            }
            type = TOKEN_LOGICAL_NOT;
            break;
        }
        case '<': {
            if (peek_char() == '=') {
                next_char();
                type = TOKEN_LESS_EQUAL;
                break;
            }
            type = TOKEN_LESS;
            break;
        }
        case '>': {
            if (peek_char() == '=') {
                next_char();
                type = TOKEN_GREATER_EQUAL;
                break;
            }
            type = TOKEN_GREATER;
            break;
        }
        case '&': {
            if (peek_char() == '&') {
                next_char();
                type = TOKEN_LOGICAL_AND;
                break;
            }
            type = TOKEN_AMPERSAND;
            break;
        }
        case '|': {
            if (peek_char() == '|') {
                next_char();
                type = TOKEN_LOGICAL_OR;
                break;
            }
            type = TOKEN_PIPE;
            break;
        }
    }
    lexeme = file.content.substr(start, position - start);
    return create_token(lexeme, start_line, start_column, type);
}


// Main
Token Lexer::next_token() {
    skip_untracked();
    char c = peek_char();

    if (isdigit(c)) return lex_number();
    if (isalpha(c) || c == '_') return lex_identifier();
    if (c == '\"') return lex_string();
    if (c == '\'') return lex_char();

    return lex_operator();
}

void Lexer::run() {

    while (true) {
        tokens.push_back(next_token());
        if (at_end()) break;
    }
}

Lexer::Lexer(const std::string& path)
    : position(0), line(1), column(1), file(get_content(path))
{
    keywords = {
        {"import", TOKEN_IMPORT},
        {"if", TOKEN_IF},
        {"else", TOKEN_ELSE},
        {"for", TOKEN_FOR},
        {"while", TOKEN_WHILE},
        {"break", TOKEN_BREAK},
        {"continue", TOKEN_CONTINUE},
        {"return", TOKEN_RETURN},
        {"struct", TOKEN_STRUCT},
        {"class", TOKEN_CLASS},
        {"public", TOKEN_PUBLIC},
        {"private", TOKEN_PRIVATE},
        {"new", TOKEN_NEW},
        {"delete", TOKEN_DELETE},
        {"true", TOKEN_TRUE},
        {"false", TOKEN_FALSE},
        {"null", TOKEN_NULL},
        {"const", TOKEN_CONST},
        {"void", TOKEN_VOID},
        {"match", TOKEN_MATCH},
        {"enum", TOKEN_ENUM},
    };
}
Lexer::~Lexer() {
    std::cout << "Lexer destroyed" << std::endl;
}

std::vector<Token>& Lexer::get_tokens() {
    return tokens;
}

const std::vector<Token>& Lexer::get_tokens() const {
    return tokens;
}
