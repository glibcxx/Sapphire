#pragma once

#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "components/BlockComponentStorage.h"
#include "BlockSerializationId.h"
#include "BlockLegacy.h"

class Block {
public:
    ;                                          // off+(1.21.2), (1.21.50/1.21.60)
    BlockComponentStorage        mComponents;  // off+8
    const DataID                 mData;        // off+40, 112
    gsl::not_null<BlockLegacy *> mLegacyBlock; // off+48, 120

    // ...

    // vtb+0
    virtual ~Block() = default;
};