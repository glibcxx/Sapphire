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

    // size: 104
    class StorageAreaStateListener {
    public:
        std::shared_ptr<FileStorageArea> mFileStorageArea; // off+8
        Bedrock::Threading::Mutex        mMutex;           // off+24

        // vtb+0
        virtual ~StorageAreaStateListener();

        // vtb+1
        virtual void onExtendDiskSpace(
            const bool                      bSet,
            std::weak_ptr<FileStorageArea> &fileStorageAreaWeakPtr,
            uint64_t                        freeSpace,
            std::function<void()>           onHandledEventCallback
        );

        // vtb+2
        virtual void onLowDiskSpace(const bool bSet);

        // vtb+3
        virtual void onOutOfDiskSpace(const bool bSet);

        // vtb+4
        virtual void onCriticalDiskError(const bool bSet, const LevelStorageState &errorCode);
    };

} // namespace Core