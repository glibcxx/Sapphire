#pragma once

#include <functional>
#include <unordered_map>
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src/common/world/phys/Vec3.h"

class Block;
class Actor;

// size: 80
class ListenerInfo {
public:
    std::function<void(::BlockPos const &, uint32_t, const Block &, Actor *)> mCallback;  // off+0
    Vec3                                                                      mPosition;  // off+64
    float                                                                     mRadiusSqr; // off+76
};

// size: 72
class BlockEventDispatcher : public Bedrock::EnableNonOwnerReferences {
public:
    using CallbackFunction = std::function<void(const BlockPos &, uint32_t, const Block &)>;

    std::unordered_map<int, std::unique_ptr<ListenerInfo>> mRegisteredListeners; // off+0
    int                                                    mHandleCounter;       // off+64
};