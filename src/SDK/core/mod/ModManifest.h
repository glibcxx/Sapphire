#pragma once

/*
    对应 mod 的清单文件，一个清单文件大致如下：
    ```
    {
        "format_version": 2,
        "id": "sapphire:builtin_mod",
        "type": "mod"
        "name": "Builtin Mod",
        "version": "0.2.3",
        "author": "Sapphire Project",
        "description": "The Builtin Mod in Sapphire Project.",
        "dependencies": {
            "sapphire:core": ">=0.2.3",
            "minecraft:game": "1.21.50"
        },
        "bin": ["builtin_mod+mc1.21.50.dll"]
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
        enum class ModType : uint32_t {
            SapphireMod = 0,
            Runtime = 1
        };

        std::string                        mId;
        ModType                            mType;
        SemanticVersion                    mVersion;
        std::vector<std::filesystem::path> mBinPath;
        std::vector<ModDependency>         mDeps;
    };

} // namespace sapphire::core