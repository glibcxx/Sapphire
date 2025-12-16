#pragma once

#include <type_traits>

namespace sapphire::coro {

    template <typename T>
    using RemoveRValueReference = std::conditional_t<std::is_rvalue_reference_v<T>, std::remove_reference_t<T>, T>;

}