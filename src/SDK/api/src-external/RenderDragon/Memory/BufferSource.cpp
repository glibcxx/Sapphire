#include "BufferSource.h"
#include "SDK/core/ApiManager.h"

void dragon::memory::_anonymous_namespace_at_BgfxMemoryHelpers_cpp_::BgfxDataWrapper<
    dragon::memory::BufferSource::UseTracker>::_bgfxDeleter(void *a1, void *userData) {
    using Hook = sapphire::ApiLoader<
        "\x48\x85\xD2\x74\x00\x57\x48\x83\xEC"_sig, // 1.21.50
        &BgfxDataWrapper<dragon::memory::BufferSource::UseTracker>::_bgfxDeleter>;
    return (Hook::origin)(a1, userData);
}