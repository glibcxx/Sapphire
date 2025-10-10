#pragma once

#include <chrono>
#include <memory>
#include "SDK/api/src/common/network/NetEventCallback.h"
#include "SDK/api/src/common/world/level/ChunkPos.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

#if MC_VERSION == v1_21_60
#    include "SDK/api/src-deps/Core/Threading/TaskGroup.h"
#endif

class IClientInstance;
class NetworkHandler;
class PacketSender;
class PrivateKeyManager;
class MinecraftCommands;
class Level;
class ILevel;
class BlockSource;
class Dimension;
namespace ClientBlobCache {
    class Cache;
}

#if MC_VERSION == v1_21_2
// size: 304
class ClientNetworkHandler : public NetEventCallback {
public:
    virtual ~ClientNetworkHandler() override {}

    std::shared_ptr<ClientBlobCache::Cache> mBlobCache; // off+24
    std::shared_ptr<void>                   mUnk40;     // off+40
    std::shared_ptr<void>                   mUnk56;     // off+56
    IClientInstance                        &mClient;    // off+72
    std::weak_ptr<void>                     mUnk80;     // off+80

    NetworkHandler    &mNetworkHandler;    // off+96
    PacketSender      &mPacketSender;      // off+104
    PrivateKeyManager &mClientKeys;        // off+112
    MinecraftCommands &mMinecraftCommands; // off+120
    uint64_t           mUnk128;            // off+128

    Bedrock::NonOwnerPointer<int> mLevel; // off+136

    bool mHasMessage; // off+152
    bool mIsLoggedIn; // off+153

    std::shared_ptr<bool> mExistenceTracker; // off+160

    using ChunkKey = std::pair<const Dimension *, ChunkPos>;

    uint64_t mPendingChunks[8]; // off+176, std::unordered_map<ChunkKey, uint64_t /*todo: hasher*/>

    using ChunkCallbackKey = std::tuple<NetworkIdentifier, const Dimension *, ChunkPos>;

    uint64_t mConnectionPausedCallbacks[8]; // off+240, std::unordered_map<ChunkCallbackKey, std::function<void(BlockSource &)> /*todo: hasher*/>
};
static_assert(sizeof(ClientNetworkHandler) == 304);

#elif MC_VERSION == v1_21_50
// size: 320
class ClientNetworkHandler : public NetEventCallback {
public:
    std::shared_ptr<ClientBlobCache::Cache> mBlobCache; // off+24
    std::shared_ptr<void>                   mUnk40;     // off+40
    std::shared_ptr<void>                   mUnk56;     // off+56
    void                                   *mUnk72;     // off+72
    IClientInstance                        &mClient;    // off+80
    std::weak_ptr<void>                     mUnk80;     // off+88

    NetworkHandler    &mNetworkHandler;    // off+104
    PacketSender      &mPacketSender;      // off+112
    PrivateKeyManager &mClientKeys;        // off+120
    MinecraftCommands &mMinecraftCommands; // off+128
    uint64_t           mUnk136;            // off+136

    Bedrock::NonOwnerPointer<ILevel> mLevel; // off+144

    bool mHasMessage; // off+168
    bool mIsLoggedIn; // off+169

    std::shared_ptr<bool> mExistenceTracker; // off+176

    using ChunkKey = std::pair<const Dimension *, ChunkPos>;

    uint64_t mPendingChunks[8]; // off+192, std::unordered_map<ChunkKey, uint64_t /*todo: hasher*/>

    using ChunkCallbackKey = std::tuple<NetworkIdentifier, const Dimension *, ChunkPos>;

    uint64_t mConnectionPausedCallbacks[8]; // off+256, std::unordered_map<ChunkCallbackKey, std::function<void(BlockSource &)> /*todo: hasher*/>

    // vtb+0
    virtual ~ClientNetworkHandler() override;
};
static_assert(sizeof(ClientNetworkHandler) == 320);

#elif MC_VERSION == v1_21_60
// size: 392
class ClientNetworkHandler : public NetEventCallback {
public:
    virtual ~ClientNetworkHandler() override {}

    std::shared_ptr<ClientBlobCache::Cache> mBlobCache; // off+24
    std::shared_ptr<void>                   mUnk40;     // off+40
    std::shared_ptr<void>                   mUnk56;     // off+56
    void                                   *mUnk72;     // off+72
    IClientInstance                        &mClient;    // off+80
    std::weak_ptr<void>                     mUnk80;     // off+88

    NetworkHandler    &mNetworkHandler;    // off+104
    PacketSender      &mPacketSender;      // off+112
    PrivateKeyManager &mClientKeys;        // off+120
    MinecraftCommands &mMinecraftCommands; // off+128
    uint64_t           mUnk136;            // off+136

    Bedrock::NonOwnerPointer<ILevel> mLevel; // off+144

    bool mHasMessage; // off+168
    bool mIsLoggedIn; // off+169

    std::shared_ptr<bool> mExistenceTracker; // off+176

    using ChunkKey = std::pair<const Dimension *, ChunkPos>;

    uint64_t mPendingChunks[8]; // off+192, std::unordered_map<ChunkKey, uint64_t /*todo: hasher*/>

    using ChunkCallbackKey = std::tuple<NetworkIdentifier, const Dimension *, ChunkPos>;

    uint64_t                   mConnectionPausedCallbacks[8]; // off+256, std::unordered_map<ChunkCallbackKey, std::function<void(BlockSource &)> /*todo: hasher*/>
    uint64_t                   mUnkMap320[8];                 // off+320, std::unordered_map<???>
    std::unique_ptr<TaskGroup> mFileDownloadManager;          // off+384
};
static_assert(sizeof(ClientNetworkHandler) == 392);
#endif