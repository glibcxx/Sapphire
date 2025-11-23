#include "BufferSource.h"
#include "SDK/core/SymbolResolver.h"

void dragon::memory::_anonymous_namespace_at_BgfxMemoryHelpers_cpp_::BgfxDataWrapper<
    dragon::memory::BufferSource::UseTracker>::_bgfxDeleter(void *a1, void *userData) {
    using Bind = sapphire::bind::Fn<
        "\x48\x85\xD2\x74\x00\x57\x48\x83\xEC"_sig, // 1.21.50
        &BgfxDataWrapper<dragon::memory::BufferSource::UseTracker>::_bgfxDeleter>;
    return (Bind::origin)(a1, userData);
}