#pragma once

#include "SDK/api/src-deps/Core/Platform/Result.h"
#include "SDK/api/src-deps/Core/Utility/UniqueOwnerPointer.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src-deps/Core/File/Path.h"

class LevelData;
class LevelStorage;
class Scheduler;
class ContentIdentity;
class IContentKeyProvider;
class LevelDbEnv;
class LevelStorageEventing;
class LevelLooseFileStorage;

// size: 24
class LevelStorageSource : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~LevelStorageSource() override = default;

    // vtb+1
    virtual const std::string &getName() const = 0;

    // vtb+2
    virtual Core::Result getLevelData(const std::string &levelId, LevelData &levelDataOut) const = 0;

    // vtb+3
    virtual void saveLevelData(const std::string &levelId, const LevelData &data) = 0;

    // vtb+4
    virtual void getLevelList(std::vector<Core::PathBuffer<std::string>> &paths) = 0;

    // vtb+5
    virtual Bedrock::UniqueOwnerPointer<LevelStorage>
    createLevelStorage(
        Scheduler                                                    &scheduler,
        const std::string                                            &levelId,
        const ContentIdentity                                        &contentIdentity,
        const Bedrock::NotNullNonOwnerPtr<const IContentKeyProvider> &keyProvider,
        const std::chrono::nanoseconds                               &writeFlushInterval,
        Bedrock::NotNullNonOwnerPtr<LevelDbEnv>                       levelDbEbv,
        std::unique_ptr<LevelStorageEventing>                         eventing
    ) = 0;

    // vtb+6
    virtual std::unique_ptr<LevelLooseFileStorage>
    createLevelLooseStorage(
        const std::string                                            &levelId,
        const ContentIdentity                                        &contentIdentity,
        const Bedrock::NotNullNonOwnerPtr<const IContentKeyProvider> &keyProvider
    ) = 0;

    // vtb+7
    virtual void deleteLevel(const std::string &levelId) = 0;

    // vtb+9
    virtual bool renameLevel(const std::string &levelId, const std::string &newLevelName) = 0;

    // vtb+8
    virtual void renameLevel(LevelData &levelData, const Core::Path &fullPath, const std::string &newLevelName) = 0;

    // vtb+10
    virtual bool createBackupCopyOfWorld(const std::string &levelId, const std::string &newLeveId, const std::string &newName) = 0;

    // vtb+11
    virtual bool isLevelMarkedForSync(const Core::Path &levelPath) const = 0;

    // vtb+12
    virtual bool isLevelPartiallyCopied(const Core::Path &levelPath) const = 0;

    // vtb+13
    virtual Core::PathBuffer<std::string> getLevelDatFoundPath(const Core::Path &levelPath) const = 0;

    // vtb+14
    virtual const Core::PathBuffer<std::string> getBasePath() const = 0;

    // vtb+15
    virtual const Core::PathBuffer<std::string> getPathToLevel(const std::string &levelId) const = 0;

    // vtb+16
    virtual const Core::PathBuffer<std::string> getPathToLevelInfo(const std::string &levelId, bool forceInfo) const = 0;

    // vtb+17
    virtual bool isBetaRetailLevel(const std::string &levelId) const = 0;
};