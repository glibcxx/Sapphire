#pragma once

#include <memory>
#include <vector>
#include "SDK/api/src/common/util/NewType.h"

template <typename T>
using Unique = std::unique_ptr<T>;
template <typename T>
using Shared = std::shared_ptr<T>;
template <typename T>
using Weak = std::weak_ptr<T>;

using ActorList = std::vector<class Actor *>;
using BlockActorList = std::vector<class BlockActor *>;
using OwnedActorList = std::vector<std::unique_ptr<Actor>>;
using PlayerList = std::vector<class Player *>;
using MobEffectInstanceList = std::vector<class MobEffectInstance>;
using SubClientId = uint8_t;
using RandomSeed = uint32_t;
using Height = int16_t;

using FacingID = uint8_t;
using BlockRuntimeId = uint32_t;

struct NewBlockID : NewType<uint16_t> {
};

struct BlockID : NewType<uint8_t> {
};

struct Brightness : NewType<uint8_t> {
    static const Brightness INVALID;
    static const Brightness MAX;
    static const Brightness MIN;
    static const uint8_t    NUM_VALUES = 16;
};

struct BrightnessPair {
    Brightness sky;
    Brightness block;
};

using DataID = uint16_t;