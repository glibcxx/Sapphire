#include "Scheduler.h"

Scheduler *Scheduler::ctor(std::string name, uint32_t FPS) {
    using Hook = sapphire::ApiLoader<
        "\xE8\x00\x00\x00\x00\x90\x0F\x57\xC0\xF3\x0F\x7F\x45\x00\x44\x8B\xE3"_sig,
        &Scheduler::ctor,
        sapphire::deRefCall>;
    return (this->*Hook::origin)(std::move(name), FPS);
}