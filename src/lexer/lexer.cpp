#include "lexer.h"
#include <cctype>
#include <typeindex>
#include <vector>
#include <stdexcept>


bool Lexer::at_end() {
    return position >= file.content.length();
}
char Lexer::next_char() {
    if (at_end()) return '\0';
    return file.content.at(position++);
}
char Lexer::peek_char() {
    if (position >= file.content.length()) return '\0';
    return file.content.at(position);
}
Token Lexer::create_token(std::string& lexeme, TokenType type) {
    return Token{lexeme, line, column, type};
}
TokenType Lexer::is_keyword(std::string s) {
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
            // consume the '//' then skip to end of line
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
    while (!at_end() && (isalnum(file.content.at(position)) || file.content.at(position) == '_')) {
        next_char();
    }
    std::string lexeme = file.content.substr(start, position);
    return create_token(lexeme, TOKEN_IDENT);
}
Token Lexer::lex_number() {
    uint32_t start = position;
    bool is_float;
    while (!at_end()) {
        char c = peek_char();
        if (isdigit(c)) {
            next_char();
        } else if (c == '.') {
            if (is_float) {
                // error too many decimal points
                break;
            }
            if (isdigit(file.content.at(position+1))) {
                is_float = 1;
                next_char();
            } else {
                // error trailing decimal
                break;
            }
        } else {
            break;
        }
    }
    std::string lexeme = file.content.substr(start, position);
    return create_token(lexeme, is_float ? TOKEN_FLOAT_LITERAL : TOKEN_INT_LITERAL);
}
Token Lexer::lex_char() {
    return{};
}
Token Lexer::lex_string() {
    return{};
}
Token Lexer::lex_operator() {
    return{};
}


// Main
Token Lexer::next_token() {
    skip_untracked();
    char c = peek_char();

    if (isdigit(static_cast<unsigned char>(c))) return lex_number();
    if (isalpha(static_cast<unsigned char>(c)) || c == '_') return lex_identifier();
    if (c == '\"') return lex_string();
    if (c == '\'') return lex_char();

    return lex_operator();
}

void Lexer::run() {

    while (true) {
        tokens.push_back(next_token());
        if (at_end()) break;
        position++;
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

}
