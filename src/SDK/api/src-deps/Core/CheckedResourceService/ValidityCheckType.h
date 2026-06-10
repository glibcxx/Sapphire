#pragma once

#include "pch.h" // IWYU pragma: keep

namespace mce {

    enum class ValidityCheckType : uint8_t {
        Increment = 0,
        Immutable = 1,
    };

}
