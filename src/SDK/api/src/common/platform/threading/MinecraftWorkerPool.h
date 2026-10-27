#pragma once

#include "SDK/api/src-deps/Platform/Threading/ThreadUtil.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class WorkerPool;

// size: 32
struct ThreadConfiguration {
public:
    Bedrock::Threading::OSThreadPriority Priority = Bedrock::Threading::OSThreadPriority::Normal; // off+0
    std::optional<uint64_t>              CoreAffinityMask{};                                      // off+8
    int32_t                              IdealCore = 0;                                           // off+24
};

// size: 40
struct WorkerConfiguration : public ThreadConfiguration {
public:
    unsigned NumWorkers = 0;            // off+32
    bool     SuppressProfiling = false; // off+36
};

// size: 608 (1.21.50)
struct AllWorkerConfigurations {
public:
    ThreadConfiguration MainThread;        // off+0
    ThreadConfiguration ServerThread;      // off+32
    ThreadConfiguration AudioThreads;      // off+64
    ThreadConfiguration Unk;               // off+96
    WorkerConfiguration Async;             // off+128
    WorkerConfiguration Disk;              // off+168
    WorkerConfiguration Network;           // off+208
    WorkerConfiguration Rendering;         // off+248
    WorkerConfiguration LevelDB;           // off+288
    WorkerConfiguration LevelDBCompaction; // off+328
    WorkerConfiguration ConnectedStorage;  // off+368
    WorkerConfiguration Watchdog;          // off+408
    WorkerConfiguration VR;                // off+448
    WorkerConfiguration PythonRunTime;     // off+488
    WorkerConfiguration Input;             // off+528
    WorkerConfiguration GamefaceLayout;    // off+568

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xC7\x85\x00\x00\x00\x00\x00\x00\x00\x00\xC7\x85\x00\x00\x00\x00\x00\x00\x00\x00\xC7\x85\x00\x00\x00\x00\x00\x00\x00\x00\xC7\x85\x00\x00\x00\x00\x00\x00\x00\x00\x44\x89\xA5")
    SDK_API AllWorkerConfigurations();

    SPHR_CTOR_ALIAS SDK_API AllWorkerConfigurations *ctor();
};

// size: 1
class MinecraftWorkerPool {
public:
    struct Pools;

    SPHR_DECL_API("1.21.50", "lea", "\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x89\x75\x00\x48\x8B\x75\x00\x49\x8B\xFE\x4C\x89\x75\x00\xBB")
    SDK_API static Pools Pools;

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x07\x48\x8B\xCF\x48\x8B\x80\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\xE8")
    SDK_API static void createSingletons();
};

struct MinecraftWorkerPool::Pools {
    static Bedrock::NonOwnerPointer<WorkerPool> ASYNC;
    static Bedrock::NonOwnerPointer<WorkerPool> DISK;
    static Bedrock::NonOwnerPointer<WorkerPool> NETWORK;
    static Bedrock::NonOwnerPointer<WorkerPool> RENDERING;
    static Bedrock::NonOwnerPointer<WorkerPool> RENDERING_WORKERS;
    static Bedrock::NonOwnerPointer<WorkerPool> LEVELDB;
    static Bedrock::NonOwnerPointer<WorkerPool> LEVELDB_COMPACTION;
    static Bedrock::NonOwnerPointer<WorkerPool> CONNECTEDSTORAGE;
    static Bedrock::NonOwnerPointer<WorkerPool> WATCHDOG;
    static Bedrock::NonOwnerPointer<WorkerPool> VR;
    static Bedrock::NonOwnerPointer<WorkerPool> PYTHON_RUNTIME;
    static Bedrock::NonOwnerPointer<WorkerPool> INPUT;
    static Bedrock::NonOwnerPointer<WorkerPool> GAMEFACE_LAYOUT;
};

SPHR_DECL_API("1.21.50", "lea", "\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x4D\x00\x49\x8B\xD5")
SDK_API extern std::optional<AllWorkerConfigurations> WorkerConfigurations;
