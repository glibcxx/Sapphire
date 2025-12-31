#pragma once

#include "SDK/api/src/common/server/commands/Command.h"
#include "SDK/api/src/common/world/actor/ActorDefinitionIdentifier.h"

class CommandOrigin;
class Actor;
struct ActorDefinitionIdentifier;

template <typename T>
struct InvertableFilter {
    T    value; // off+0
    bool inverted;
};

enum class CommandSelectionType : int {
    Self = 0,
    Entities = 1,
    Players = 2,
    DefaultPlayers = 3,
    OwnedAgent = 4,
    Agents = 5,
    Initiator = 6,
};

enum class CommandSelectionOrder : int {
    Sorted = 0,
    InverseSorted = 1,
    Random = 2,
};

using CommandResultVector = std::shared_ptr<std::vector<Actor *>>;

class CommandSelectorBase {
public:
    using FilterFunc = std::function<bool(const CommandOrigin &, const Actor &)>;

    int                                                      mVersion = 0;                                   // off+0
    CommandSelectionType                                     mType = CommandSelectionType::Self;             // off+4
    CommandSelectionOrder                                    mOrder = CommandSelectionOrder::Sorted;         // off+8
    std::vector<InvertableFilter<std::string>>               mNameFilters;                                   // off+16
    std::vector<InvertableFilter<ActorDefinitionIdentifier>> mTypeFilters;                                   // off+40
    std::vector<InvertableFilter<HashedString>>              mFamilyFilters;                                 // off+64
    std::vector<InvertableFilter<std::string>>               mTagFilters;                                    // off+88
    std::vector<CommandSelectorBase::FilterFunc>             mFilterChain;                                   // off+112
    CommandPosition                                          mPosition{};                                    // off+136
    BlockPos                                                 mBoxDeltas{};                                   // off+152
    float                                                    mRadiusMin = 0.0f;                              // off+164
    float                                                    mRadiusMax = std::numeric_limits<float>::max(); // off+168
    size_t                                                   mCount = std::numeric_limits<size_t>::max();    // off+176
    bool                                                     mIncludeDeadPlayers = false;                    // off+184
    bool                                                     mIsPositionBound = false;                       // off+185
    bool                                                     mDistanceFiltered = false;                      // off+186
    bool                                                     mPositionFiltered = false;                      // off+187
    bool                                                     mCountFiltered = false;                         // off+188
    bool                                                     mHaveDeltas = false;                            // off+189
    bool                                                     mForcePlayer = false;                           // off+190
    bool                                                     mExcludeAgents = false;                         // off+191
    bool                                                     mIsExplicitIdSelector = false;                  // off+192
    bool                                                     mForceDimensionFiltering = false;               // off+193

    CommandSelectorBase(bool forcePlayer) : mForcePlayer(forcePlayer) {}

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF8\x4C\x8B\xEA\x48\x89\x95")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x48\x00\x48\x85\xC9\x74\x00\xF0\xFF\x41\x00\x4C\x8B\x38\x49\x8B\xCF")
    SDK_API CommandResultVector newResults(const CommandOrigin &origin) const;
};

template <typename T>
class CommandSelectorResults {
public:
    CommandResultVector mTargets; // off+0

    CommandSelectorResults(CommandResultVector targets) :
        mTargets(targets) {}
    CommandSelectorResults(const CommandSelectorResults &results) = default;

    std::vector<T>::iterator begin() const { return mTargets->begin(); }
    std::vector<T>::iterator end() const { return mTargets->end(); }

    T     *get() const { return static_cast<T *>(mTargets->front()); }
    bool   empty() const { return mTargets->empty(); }
    size_t size() const { return mTargets->size(); }
};

template <typename T>
class CommandSelector : CommandSelectorBase {
public:
    CommandSelector() : CommandSelectorBase(std::is_same_v<T, Player>) {}

    CommandSelectorResults<T> results(const CommandOrigin &origin) const {
        return this->newResults(origin);
    }
};