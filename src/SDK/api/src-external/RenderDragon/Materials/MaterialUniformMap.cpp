#include "MaterialUniformMap.h"

namespace dragon::materials {

    MaterialUniformMap *MaterialUniformMap::ctor(const MaterialUniformMap &other, Core::CheckedRingBuffer<2, 0> &allocator) {
        using Fn = MaterialUniformMap *(MaterialUniformMap::*)(const MaterialUniformMap &, Core::CheckedRingBuffer<2, 0> &);
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\x48\x89\xB5\x00\x00\x00\x00\x48\x8B\x4D"_sig,
            (Fn)&MaterialUniformMap::ctor,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(other, allocator);
    }

    SDK_API MaterialUniformMap *MaterialUniformMap::ctor(
        const CompiledMaterialDefinition &definition,
        MaterialUniformHandles           &uniformHandles,
        Core::CheckedRingBuffer<2, 0>    &allocator
    ) {
        using Fn = MaterialUniformMap *(MaterialUniformMap::*)(const CompiledMaterialDefinition &, MaterialUniformHandles &, Core::CheckedRingBuffer<2, 0> &);
        using Hook = sapphire::ApiLoader<
            "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xD8\x4C\x8B\xE2\x48\x89\x54\x24"_sig,
            (Fn)&MaterialUniformMap::ctor,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(definition, uniformHandles, allocator);
    }

} // namespace dragon::materials
