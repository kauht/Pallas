#pragma once

#include <cstdint>

namespace pallas::frontend {

enum class NodeType : std::uint8_t {
    NODE_ROOT,

    NODE_FUNCTION,
    NODE_BLOCK,
    NODE_RETURN,
    NODE_CONTINUE,
    NODE_BREAK,
    NODE_VAR_DECL,
    NODE_CONST,
};
}  // namespace pallas::frontend
