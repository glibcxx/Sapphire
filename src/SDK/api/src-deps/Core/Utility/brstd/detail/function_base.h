#pragma once

namespace brstd::detail {

    constexpr size_t max_small_obj_ptr_count = 8;
    constexpr size_t max_small_obj_size = (detail::max_small_obj_ptr_count - 1) * sizeof(void *);

} // namespace brstd::detail