#include "Scheduler.h"

Scheduler *Scheduler::ctor(std::string name, uint32_t FPS) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xD8\x48\x8B\x0D\x00\x00\x00\x00\x48\x89\x1D"_sig,
        &Scheduler::ctor>;
    return (this->*Bind::origin)(std::move(name), FPS);
}