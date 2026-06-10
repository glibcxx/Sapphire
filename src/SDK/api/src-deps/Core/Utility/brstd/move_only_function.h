#pragma once

#include "pch.h" // IWYU pragma: keep

#include "detail/function_base.h"

namespace brstd {

    // TODO: implement this shit

    template <typename T>
    class move_only_function {
        static_assert(false, "Template Argument must be a function type");
    };

    // size: 64
    template <typename Ret, typename... Args>
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