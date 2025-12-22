#pragma once

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