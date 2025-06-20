#include "MaterialUniformMap.h"

namespace dragon::materials {

    MaterialUniformMap *MaterialUniformMap::ctor(const MaterialUniformMap &other, uint64_t allocator) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x48\x89\xB5\x00\x00\x00\x00\x48\x8B\x4D"_sig,
            &MaterialUniformMap::ctor,
            core::deRefCall>;
        return (this->*Hook::origin)(other, allocator);
    }

} // namespace dragon::materials
