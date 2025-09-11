#include "RenderChunkInstanced.h"

RenderChunkInstanced *RenderChunkInstanced::ctor() {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xDF\x48\x8D\x43\x00\x48\x89\x45"_sig, // 1.21.50
        &RenderChunkInstanced::ctor>;
    return (this->*Hook::origin)();
}
