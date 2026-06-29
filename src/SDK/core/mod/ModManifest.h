#pragma once

/*
    对应 mod 的清单文件，一个清单文件大致如下：
    ```
    {
        "format_version": 1,
        "id": "Sapphire:builtin_mod",
        "name": "Builtin Mod",
        "version": "0.1.1",
        "author": "Sapphire Project",
        "description": "The Builtin Mod in Sapphire Project.",
        "dependencies": {
            "Sapphire:core": ">=0.2.0",
            "minecraft": "1.21.50"
        },
        "main_dll": "builtin_mod+mc1.21.50.dll"
    }
    ```
*/

#include "pch.h" // IWYU pragma: keep

#include "common/SemanticVersion.hpp"

namespace sapphire::core {

    struct ModDependency {
        std::string mId;
        std::string mVersionConstraint;
    };

    struct ModManifest {
        std::string                mId;
        SemanticVersion            mVersion;
        std::filesystem::path      mMainDllPath;
        std::vector<ModDependency> mDeps;
    };

} // namespace sapphire::core