#pragma once

#include <unordered_set>
#include "SDK/api/src-deps/Coregraphics/ResourceManager.h"
#include "CompiledMaterialDefinition.h"
#include "Material.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonMaterialResourceService.h"

class TaskGroup;

namespace dragon::materials {

    struct MaterialResource {
        std::shared_ptr<mce::ResourcePointer<dragon::ResolvedMaterialResource>> mMaterial; // off+0
        bool                                                                    mUsed;     // off+16
    };

    struct MaterialLocation {
        const uint64_t mHash; // off+0
    };

    // size: 232 (1.21.50)
    class MaterialResourceManager {
    public:
        std::unordered_map<MaterialLocation, MaterialResource> mUnk0;   // off+0
        std::unordered_set<MaterialLocation>                   mUnk64;  // off+64
        std::vector<std::shared_ptr<MaterialResource>>         mUnk128; // off+128
        std::mutex                                             mMutex;  // off+152

        MaterialResourceManager *ctor(uintptr_t a1); // \xE8\x00\x00\x00\x00\x90\x48\x8B\x95\x00\x00\x00\x00\x48\x89\x85 1.21.50
    };

} // namespace dragon::materials