#include "MinecraftScheduler.h"

Scheduler &MinecraftScheduler::client() {
    using Hook = sapphire::ApiLoader<
        "\xE8\x00\x00\x00\x00\xF2\x0F\x10\x05\x00\x00\x00\x00\xF2\x0F\x11\x40\x00\xE8"_sig,
        &MinecraftScheduler::client,
        sapphire::deRefCall>;
    return Hook::origin();
}