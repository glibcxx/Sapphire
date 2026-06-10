#pragma once

#include "pch.h" // IWYU pragma: keep

namespace dragon::rendering {

    enum class LightingModels : int {
        Vanilla = 0,
        Deferred = 1,
        FullPathTraced = 2,
    };

}
