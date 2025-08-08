#include "Scheduler.h"

Scheduler *Scheduler::ctor(std::string name, uint32_t FPS) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xD8\x48\x8B\x0D\x00\x00\x00\x00\x48\x89\x1D"_sig,
        &Scheduler::ctor>;
    return (this->*Hook::origin)(std::move(name), FPS);
}