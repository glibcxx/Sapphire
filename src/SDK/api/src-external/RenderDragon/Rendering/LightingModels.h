#pragma once

namespace dragon::rendering {

    enum class LightingModels : int {
        Vanilla = 0,
        Deferred = 1,
        FullPathTraced = 2,
    };

}
