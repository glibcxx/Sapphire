#pragma once

#include "pch.h" // IWYU pragma: keep

namespace dragon::mesh {

    enum class IndexSize : uint8_t {
        Bytes16 = 0,
        Bytes32 = 1,
    };

}