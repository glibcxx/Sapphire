#pragma once

#include "SDK/api/src-deps/Core/Utility/brstd/flat_map.h"
#include "SDK/api/src-deps/Core/Utility/brstd/flat_set.h"
#include "SDK/api/src-deps/Core/Utility/TypeId.h"

// https://github.com/LiteLDev/LeviLamina/blob/c012794923f9bc3d388dd0cf7e133c7b1d280d3c/src-server/mc/world/level/block/components/BlockComponentStorage.h#L53

// size: 40 (1.21.2), 104 (1.21.50/1.21.60)
class BlockComponentStorage {
public:
    struct ComponentBase {
        // vtb+0
        virtual ~ComponentBase() = default;
    };

#if MC_VERSION == v1_21_2
    std::vector<std::pair<Bedrock::typeid_t<void>, std::unique_ptr<ComponentBase>>> mComponents; // off+0
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    brstd::flat_map<Bedrock::typeid_t<void>, std::unique_ptr<ComponentBase>> mComponents;          // off+0
    brstd::flat_set<Bedrock::typeid_t<void>>                                 mStatelessComponents; // off+56
#endif

    ;                                             // off+(1.21.2), (1.21.50/1.21.60)
    bool mAllowModifyingComponents;               // off+32, 96
    bool mAllowComponentReplacement;              // off+33, 97
    bool mAllowTryGetComponentBeforeFinalization; // off+34, 98
};