#pragma once

#include <cstdint>
#include <string>

namespace pallas::frontend {

enum class ErrorCode : std::uint8_t {
    E101_UNTERMINATED_BLOCK_COMMENT = 101,
    E103_INVALID_HEX_LITERAL = 103,
    E104_UNTERMINATED_CHAR_LITERAL = 104,
    E105_INVALID_NUMBER_LITERAL = 105,
    E107_UNTERMINATED_STRING_LITERAL = 107,
};

int error_code_value(ErrorCode code) {
    return static_cast<int>(code);
}

const char* code_to_string(ErrorCode code) {
    switch (code) {
        case ErrorCode::E101_UNTERMINATED_BLOCK_COMMENT:
            return "unterminated block comment";
        case ErrorCode::E103_INVALID_HEX_LITERAL:
            return "invalid hex literal";
        case ErrorCode::E104_UNTERMINATED_CHAR_LITERAL:
            return "unterminated character literal";
        case ErrorCode::E105_INVALID_NUMBER_LITERAL:
            return "invalid number literal";
        case ErrorCode::E107_UNTERMINATED_STRING_LITERAL:
            return "unterminated string literal";

        default:
            return "unknown error";
    }
}

}  // namespace pallas::frontend
