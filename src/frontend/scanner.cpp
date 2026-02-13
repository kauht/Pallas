#include "scanner.h"
#include <cctype>
#include <cstdint>
#include <string>
#include <utility>
#include "diagnostics.h"

namespace pallas::frontend {

void Scanner::report(Severity sev, ErrorCode code, const std::string& msg, std::size_t start_offset,
                     std::size_t pos_offset, std::size_t line_no, std::size_t col_no) {
    std::size_t length = 0;
    if (pos_offset >= start_offset) {
        length = pos_offset - start_offset;
    }
    if (diagnostics != nullptr) {
        diagnostics->report(sev, code, msg, "", start_offset, length, line_no, col_no);
    }
}

Token Scanner::next_token() {
    if (token_index < tokens.size()) {
        return tokens[token_index++];
    }

    return make_token(TokenType::TOKEN_EOF);
}

bool Scanner::is_at_end() {
    return position >= source.size();
}

char Scanner::advance() {
    if (is_at_end()) {
        return '\0';
    }
    char c = source[position++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

bool Scanner::match_char(char expected) {
    if (is_at_end()) {
        return false;
    }
    if (source[position] != expected) {
        return false;
    }
    advance();
    return true;
}

char Scanner::peek_char() {
    if (is_at_end()) {
        return '\0';
    }
    return source[position];
}

char Scanner::peek_char(uint8_t n) {
    std::size_t idx = position + n;
    if (idx >= source.size()) {
        return '\0';
    }
    return source[idx];
}

void Scanner::add_token(TokenType type) {
    tokens.push_back(make_token(type));
}

Token Scanner::make_token(TokenType type) {
    Token t;
    t.type = type;

    std::size_t tok_start = start;
    std::size_t tok_len = 0;
    if (position >= tok_start) {
        tok_len = position - tok_start;
    }

    if (tok_len > 0 && tok_start < source.size()) {
        t.lexeme = source.substr(tok_start, tok_len);
    }

    t.offset = tok_start;
    t.length = tok_len;
    t.line = start_line;
    t.column = start_column;

    return t;
}

TokenType Scanner::is_keyword(const std::string& ident) {
    auto k = keywords.find(ident);
    if (k != keywords.end()) {
        return k->second;
    }
    return TokenType::TOKEN_IDENT;
}

void Scanner::skip_untracked() {
    for (;;) {
        if (is_at_end()) {
            return;
        }

        char c = peek_char();

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
            continue;
        }

        if (c == '/' && peek_char(1) == '/') {
            advance();
            advance();
            while (peek_char() != '\n' && !is_at_end()) {
                advance();
            }
            continue;
        }

        if (c == '/' && peek_char(1) == '*') {
            advance();
            advance();
            bool closed = false;
            while (!is_at_end()) {
                if (peek_char() == '*' && peek_char(1) == '/') {
                    advance();
                    advance();
                    closed = true;
                    break;
                }
                advance();
            }
            if (!closed) {
                report(Severity::Error, ErrorCode::E101_UNTERMINATED_BLOCK_COMMENT,
                       "unterminated block comment", (position > 0 ? position - 2 : position),
                       position, line, column);
            }
            continue;
        }

        break;
    }
}

void Scanner::s_identifier() {
    while ((std::isalnum(peek_char()) != 0) || peek_char() == '_') {
        advance();
    }
    std::size_t len = position - start;
    std::string lex = source.substr(start, len);
    TokenType type = is_keyword(lex);
    add_token(type);
}

void Scanner::s_number() {
    if (peek_char() == '0' && (peek_char(1) == 'x' || peek_char(1) == 'X')) {
        advance();
        advance();
        bool has_digit = false;
        while (std::isxdigit(peek_char()) != 0) {
            has_digit = true;
            advance();
        }
        if (!has_digit) {
            report(Severity::Error, ErrorCode::E103_INVALID_HEX_LITERAL,
                   "hex literal has no digits", start, position, start_line, start_column);
        }
        add_token(TokenType::TOKEN_INT_LITERAL);
        return;
    }

    while (std::isdigit(peek_char()) != 0) {
        advance();
    }

    if (peek_char() == '.' && (std::isdigit(peek_char(1)) != 0)) {
        advance();
        while (std::isdigit(peek_char()) != 0) {
            advance();
        }
        add_token(TokenType::TOKEN_FLOAT_LITERAL);
    } else {
        add_token(TokenType::TOKEN_INT_LITERAL);
    }
}

void Scanner::s_char() {
    if (is_at_end()) {
        report(Severity::Error, ErrorCode::E104_UNTERMINATED_CHAR_LITERAL,
               "unterminated character literal", start, position, start_line, start_column);
        add_token(TokenType::TOKEN_CHAR_LITERAL);
        return;
    }

    if (peek_char() == '\\') {
        advance();
        if (!is_at_end()) {
            advance();
        }
    } else {
        if (!is_at_end()) {
            advance();
        } else {
            report(Severity::Error, ErrorCode::E104_UNTERMINATED_CHAR_LITERAL,
                   "unterminated character literal", start, position, start_line, start_column);
            add_token(TokenType::TOKEN_CHAR_LITERAL);
            return;
        }
    }

    if (peek_char() == '\'') {
        advance();
    } else {
        report(Severity::Error, ErrorCode::E104_UNTERMINATED_CHAR_LITERAL,
               "unterminated character literal", start, position, start_line, start_column);
    }
    add_token(TokenType::TOKEN_CHAR_LITERAL);
}

void Scanner::s_string() {
    bool closed = false;
    while (!is_at_end()) {
        if (peek_char() == '"') {
            closed = true;
            break;
        }
        if (peek_char() == '\\') {
            advance();
            if (!is_at_end()) {
                advance();
            }
            continue;
        }
        advance();
    }

    if (closed && !is_at_end() && peek_char() == '"') {
        advance();
        add_token(TokenType::TOKEN_STRING_LITERAL);
    } else {
        report(Severity::Error, ErrorCode::E107_UNTERMINATED_STRING_LITERAL,
               "unterminated string literal", start, position, start_line, start_column);
        add_token(TokenType::TOKEN_STRING_LITERAL);
    }
}

void Scanner::s_operator() {
    if (is_at_end()) {
        add_token(TokenType::TOKEN_EOF);
        return;
    }

    char c = source[position];

    switch (c) {
        case '.':
            if (peek_char() == '.' && peek_char(1) == '.') {
                advance();
                advance();
                advance();
                add_token(TokenType::TOKEN_ELLIPSIS);
            } else {
                advance();
                add_token(TokenType::TOKEN_DOT);
            }
            return;
        case '?':
            advance();
            add_token(TokenType::TOKEN_QUESTION);
            return;
        case '@':
            advance();
            add_token(TokenType::TOKEN_AT);
            return;
        case '(':
            advance();
            add_token(TokenType::TOKEN_LPAREN);
            return;
        case ')':
            advance();
            add_token(TokenType::TOKEN_RPAREN);
            return;
        case '{':
            advance();
            add_token(TokenType::TOKEN_LBRACE);
            return;
        case '}':
            advance();
            add_token(TokenType::TOKEN_RBRACE);
            return;
        case '[':
            advance();
            add_token(TokenType::TOKEN_LBRACKET);
            return;
        case ']':
            advance();
            add_token(TokenType::TOKEN_RBRACKET);
            return;
        case ';':
            advance();
            add_token(TokenType::TOKEN_SEMICOLON);
            return;
        case ',':
            advance();
            add_token(TokenType::TOKEN_COMMA);
            return;
        case ':':
            advance();
            if (peek_char() == ':') {
                advance();
                add_token(TokenType::TOKEN_DOUBLE_COLON);
            } else {
                add_token(TokenType::TOKEN_COLON);
            }
            return;
        case '+':
            advance();
            if (peek_char() == '+') {
                advance();
                add_token(TokenType::TOKEN_PLUS_PLUS);
            } else if (peek_char() == '=') {
                advance();
                add_token(TokenType::TOKEN_PLUS_ASSIGN);
            } else {
                add_token(TokenType::TOKEN_PLUS);
            }
            return;
        case '-':
            advance();
            if (peek_char() == '>') {
                advance();
                add_token(TokenType::TOKEN_ARROW);
            } else if (peek_char() == '-') {
                advance();
                add_token(TokenType::TOKEN_MINUS_MINUS);
            } else if (peek_char() == '=') {
                advance();
                add_token(TokenType::TOKEN_MINUS_ASSIGN);
            } else {
                add_token(TokenType::TOKEN_MINUS);
            }
            return;
        case '*':
            advance();
            if (peek_char() == '=') {
                advance();
                add_token(TokenType::TOKEN_STAR_ASSIGN);
            } else {
                add_token(TokenType::TOKEN_STAR);
            }
            return;
        case '/':
            advance();
            if (peek_char() == '=') {
                advance();
                add_token(TokenType::TOKEN_SLASH_ASSIGN);
            } else {
                add_token(TokenType::TOKEN_SLASH);
            }
            return;
        case '%':
            advance();
            add_token(TokenType::TOKEN_PERCENT);
            return;
        case '=':
            advance();
            if (peek_char() == '=') {
                advance();
                add_token(TokenType::TOKEN_EQUAL);
            } else {
                add_token(TokenType::TOKEN_ASSIGN);
            }
            return;
        case '!':
            advance();
            if (peek_char() == '=') {
                advance();
                add_token(TokenType::TOKEN_NOT_EQUAL);
            } else {
                add_token(TokenType::TOKEN_LOGICAL_NOT);
            }
            return;
        case '<':
            advance();
            if (peek_char() == '<') {
                advance();
                add_token(TokenType::TOKEN_LEFT_SHIFT);
            } else if (peek_char() == '=') {
                advance();
                add_token(TokenType::TOKEN_LESS_EQUAL);
            } else {
                add_token(TokenType::TOKEN_LESS);
            }
            return;
        case '>':
            advance();
            if (peek_char() == '>') {
                advance();
                add_token(TokenType::TOKEN_RIGHT_SHIFT);
            } else if (peek_char() == '=') {
                advance();
                add_token(TokenType::TOKEN_GREATER_EQUAL);
            } else {
                add_token(TokenType::TOKEN_GREATER);
            }
            return;
        case '&':
            advance();
            if (peek_char() == '&') {
                advance();
                add_token(TokenType::TOKEN_LOGICAL_AND);
            } else {
                add_token(TokenType::TOKEN_AMPERSAND);
            }
            return;
        case '|':
            advance();
            if (peek_char() == '|') {
                advance();
                add_token(TokenType::TOKEN_LOGICAL_OR);
            } else {
                add_token(TokenType::TOKEN_PIPE);
            }
            return;
        case '^':
            advance();
            add_token(TokenType::TOKEN_CARET);
            return;
        case '~':
            advance();
            add_token(TokenType::TOKEN_TILDE);
            return;
        default:
            advance();
            add_token(TokenType::TOKEN_IDENT);
            return;
    }
}

void Scanner::scan() {
    while (!is_at_end()) {
        skip_untracked();
        if (is_at_end()) {
            break;
        }

        start = position;
        start_line = line;
        start_column = column;

        char c = peek_char();

        if ((std::isalpha(c) != 0) || c == '_') {
            s_identifier();
            continue;
        }

        if (std::isdigit(c) != 0) {
            s_number();
            continue;
        }

        if (c == '\'') {
            advance();
            s_char();
            continue;
        }
        if (c == '"') {
            advance();
            s_string();
            continue;
        }

        s_operator();
    }

    start = position;
    start_line = line;
    start_column = column;
    add_token(TokenType::TOKEN_EOF);
}

Scanner::Scanner(std::string source_text) : source(std::move(source_text)) {
    scan();
}

}  // namespace pallas::frontend
