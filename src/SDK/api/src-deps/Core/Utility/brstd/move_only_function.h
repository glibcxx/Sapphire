#pragma once

#include <functional>

namespace brstd {

    namespace detail {
        constexpr size_t max_small_obj_ptr_count = 8;
        constexpr size_t max_small_obj_size = (detail::max_small_obj_ptr_count - 1) * sizeof(void *);
    } // namespace detail

    // TODO: implement this shit

    template <typename T>
    class move_only_function {
        static_assert(false, "Template Argument must be a function type");
    };

    // size: 64
    template <typename Ret, typename... Args>
        requires std::is_function_v<Ret(Args...)>
    class move_only_function<Ret(Args...)> {
    public:
        void *functor;
        union alignas(max_align_t) {
            std::byte dummy[detail::max_small_obj_size];
            void     *heap;
        };

        move_only_function() = delete;
    };

} // namespace brstd