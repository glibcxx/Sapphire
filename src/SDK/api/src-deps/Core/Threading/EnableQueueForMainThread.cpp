#include "EnableQueueForMainThread.h"

Bedrock::Threading::EnableQueueForMainThread *Bedrock::Threading::EnableQueueForMainThread::ctor(std::string name) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x49\x89\x6E\x00\x49\x89\x6E\x00\x48\x8D\x05"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x4C\x89\x00\x00\x4C\x89\x00\x00\x4C\x8D"_sig,
#endif
        &EnableQueueForMainThread::ctor>;
    return (this->*Hook::origin)(std::move(name));
}