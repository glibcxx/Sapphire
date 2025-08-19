#include "BedrockMemoryAllocator.h"
namespace sapphire::inline memory {

    BedrockMemoryAllocator &BedrockMemoryAllocator::getInstance() {
        static BedrockMemoryAllocator instance{};
        return instance;
    }

} // namespace sapphire::memory
