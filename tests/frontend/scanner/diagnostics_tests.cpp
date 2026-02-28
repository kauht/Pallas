#include <catch2/catch_test_macros.hpp>
#include <string>
#include "frontend/scanner.h"

using namespace pallas::frontend;

TEST_CASE("unterminated block comment produces diagnostic") {
    Diagnostics diag;
    Scanner s("p /* unclosed comment", &diag);

    REQUIRE(diag.size() >= 1);
    const Info& info = diag.all()[0];
    REQUIRE(info.code == ErrorCode::E101_UNTERMINATED_BLOCK_COMMENT);
    REQUIRE(info.message.find("unterminated block comment") != std::string::npos);
    REQUIRE(info.severity == Severity::Error);
}

TEST_CASE("invalid hex literal produces diagnostic") {
    Diagnostics diag;
    Scanner s("0x", &diag);

    REQUIRE(diag.size() >= 1);
    const Info& info = diag.all()[0];
    REQUIRE(info.code == ErrorCode::E103_INVALID_HEX_LITERAL);
    REQUIRE(info.message.find("hex literal has no digits") != std::string::npos);
    REQUIRE(info.severity == Severity::Error);
}

TEST_CASE("unterminated string literal produces diagnostic") {
    Diagnostics diag;
    Scanner s("\"not closed", &diag);

    REQUIRE(diag.size() >= 1);
    const Info& info = diag.all()[0];
    REQUIRE(info.code == ErrorCode::E107_UNTERMINATED_STRING_LITERAL);
    REQUIRE(info.message.find("unterminated string literal") != std::string::npos);
    REQUIRE(info.severity == Severity::Error);
}

TEST_CASE("unterminated char literal produces diagnostic") {
    Diagnostics diag;
    Scanner s("'", &diag);

    REQUIRE(diag.size() >= 1);
    const Info& info = diag.all()[0];
    REQUIRE(info.code == ErrorCode::E104_UNTERMINATED_CHAR_LITERAL);
    REQUIRE(info.message.find("unterminated character literal") != std::string::npos);
    REQUIRE(info.severity == Severity::Error);
}

TEST_CASE("multiple sequential diagnostics are reported in order") {
    Diagnostics diag;
    std::string code = "0x \"unterminated 'x /* unclosed";
    Scanner s(code, &diag);

    REQUIRE(diag.size() >= 4);

    std::vector<ErrorCode> expected = {
        ErrorCode::E103_INVALID_HEX_LITERAL,
        ErrorCode::E107_UNTERMINATED_STRING_LITERAL,
        ErrorCode::E104_UNTERMINATED_CHAR_LITERAL,
        ErrorCode::E101_UNTERMINATED_BLOCK_COMMENT
    };

    for (size_t i = 0; i < expected.size(); ++i) {
        INFO("diagnostic index " << i);
        REQUIRE(diag.all()[i].code == expected[i]);
    }
}