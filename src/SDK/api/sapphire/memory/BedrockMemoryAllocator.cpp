#include "BedrockMemoryAllocator.h"

namespace sapphire::memory {

    BedrockMemoryAllocator &BedrockMemoryAllocator::getInstance() {
        static BedrockMemoryAllocator instance{};
        return instance;
    }

} // namespace sapphire::memory
