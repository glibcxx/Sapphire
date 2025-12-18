#pragma once

#include <new>

namespace sapphire::coro {

#ifdef __cpp_lib_hardware_interference_size
    constexpr std::size_t CacheLineSize = std::hardware_destructive_interference_size;
#else
    constexpr std::size_t CacheLineSize = 64;
#endif

} // namespace sapphire::coro
