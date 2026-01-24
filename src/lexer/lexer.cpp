#include "lexer.h"
#include <cctype>
#include <vector>


bool Lexer::at_end() {
    return position >= file.content.length();
}
char Lexer::next_char() {
    return file.content.at(position++);
}
char Lexer::peek_char() {
    return file.content.at(position+1);
}
bool Lexer::create_token() {

}
TokenType Lexer::is_keyword(std::string s) {
    return keywords.at(s);
}
void Lexer::skip_untracked() {
    while (!at_end()) {
        char c = peek_char();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            next_char();
            continue;
        }
        if (c == '/' && file.content.at(position+2) == '/') {
            while (peek_char() != '\n' && !at_end()) {
                next_char();
            }
            continue;
        }
        if (c == '/' && file.content.at(position+2) == '*') {
            next_char();
            next_char();
            while (!(peek_char() == '*' && file.content.at(position+2) == '/') && !at_end()) {
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

}
Token Lexer::lex_number() {

}
Token Lexer::lex_char() {

}
Token Lexer::lex_string() {

}
Token Lexer::lex_operator() {

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
        position++;
        tokens.push_back(next_token());
        if (at_end()) break;
    }
}

Lexer::Lexer(std::string path)
: file(get_content(path)) {
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
