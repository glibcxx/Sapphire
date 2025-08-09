#pragma once

#include <memory>
#include <vector>
#include "SDK/api/src/common/util/NewType.h"

// size: 8
template <typename T>
using Unique = std::unique_ptr<T>;
// size: 16
template <typename T>
using Shared = std::shared_ptr<T>;
// size: 16
template <typename T>
using Weak = std::weak_ptr<T>;

// size: 24
using ActorList = std::vector<class Actor *>;
// size: 24
using BlockActorList = std::vector<class BlockActor *>;
// size: 24
using OwnedActorList = std::vector<std::unique_ptr<Actor>>;
// size: 24
using PlayerList = std::vector<class Player *>;
// size: 24
using MobEffectInstanceList = std::vector<class MobEffectInstance>;
// size: 1
using SubClientId = uint8_t;
// size: 4
using RandomSeed = uint32_t;
// size: 2
using Height = int16_t;

// size: 1
using FacingID = uint8_t;
// size: 4
using BlockRuntimeId = uint32_t;

// size: 2
struct NewBlockID : NewType<uint16_t> {
};

// size: 1
struct BlockID : NewType<uint8_t> {
};

// size: 1
struct Brightness : NewType<uint8_t> {
    static const Brightness INVALID;
    static const Brightness MAX;
    static const Brightness MIN;
    static const uint8_t    NUM_VALUES = 16;
};

// size: 2
struct BrightnessPair {
    Brightness sky;
    Brightness block;
};

// size: 2
using DataID = uint16_t;