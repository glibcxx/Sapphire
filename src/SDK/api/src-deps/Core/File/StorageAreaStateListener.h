#pragma once

#include <memory>
#include <functional>

#include "SDK/api/src-deps/Platform/Threading/Mutex.h"

namespace Core {

    class FileStorageArea;

    enum class LevelStorageState : int {
        Open = 0,
        Corrupted = 1,
        NotFound = 2,
        IOError = 3,
        NotSupported = 4,
        InvalidArguments = 5,
        Unknown = 6,
    };

    class StorageAreaStateListener {
    public:
        std::shared_ptr<Core::FileStorageArea> mFileStorageArea;
        Bedrock::Threading::Mutex              mMutex;

        virtual ~StorageAreaStateListener() = 0;

        virtual void onExtendDiskSpace(
            const bool                            bSet,
            std::weak_ptr<Core::FileStorageArea> &fileStorageAreaWeakPtr,
            uint64_t                              freeSpace,
            std::function<void()>                 onHandledEventCallback
        ) = 0;

        virtual void onLowDiskSpace(const bool bSet) = 0;

        virtual void onOutOfDiskSpace(const bool bSet) = 0;

        virtual void onCriticalDiskError(const bool bSet, const Core::LevelStorageState &errorCode) = 0;
    };

} // namespace Core