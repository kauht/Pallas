#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include "error_codes.h"

namespace pallas::frontend {

enum class Severity : std::uint8_t {
    Error,
    Warning,
    Note,
    Info
};

struct Info {
    Severity severity = Severity::Error;
    ErrorCode code = ErrorCode::E101_UNTERMINATED_BLOCK_COMMENT;
    std::string message;
    std::string file;
    std::size_t start = 0;
    std::size_t length = 0;
    std::size_t line = 0;
    std::size_t column = 0;
};

class Diagnostics {
  public:
    void report(Severity severity, ErrorCode code, const std::string& message,
                const std::string& file = "", std::size_t start = 0, std::size_t length = 0,
                std::size_t line = 0, std::size_t column = 0) {
        Info d;
        d.severity = severity;
        d.code = code;
        d.message = message;
        d.file = file;
        d.start = start;
        d.length = length;
        d.line = line;
        d.column = column;
        diagnostics_.push_back(std::move(d));
    }

    void print() const {
        std::string out;
        for (const Info& d : diagnostics_) {
            append_location(d, out);
            out.append(severity_to_string(d.severity));
            out.append(": ");
            out.append(d.message);
            out.append(" [E");
            out.append(std::to_string(error_code_value(d.code)));
            out.append("]");
            out.push_back('\n');
        }
        std::cout << out;
    }

  private:
    static void append_location(const Info& d, std::string& out) {
        if (!d.file.empty()) {
            out.append(d.file);
            out.push_back(':');
            if (d.line != 0) {
                out.append(std::to_string(d.line));
                out.push_back(':');
                out.append(std::to_string(d.column));
                out.append(": ");
            } else {
                out.append(std::to_string(d.start));
                out.append(": ");
            }
        } else {
            out.append("<input>:");
            if (d.line != 0) {
                out.append(std::to_string(d.line));
                out.push_back(':');
                out.append(std::to_string(d.column));
                out.append(": ");
            } else {
                out.append(std::to_string(d.start));
                out.append(": ");
            }
        }
    }

    static const char* severity_to_string(Severity s) {
        switch (s) {
            case Severity::Error:
                return "error";
            case Severity::Warning:
                return "warning";
            case Severity::Note:
                return "note";
            case Severity::Info:
                return "info";
            default:
                return "unknown";
        }
    }

    std::vector<Info> diagnostics_;
};

}  // namespace pallas::frontend
