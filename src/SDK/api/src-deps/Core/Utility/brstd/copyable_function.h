#pragma once

#include "detail/function_base.h"

namespace brstd {

    // TODO: implement this shit

    template <typename T>
    class copyable_function {
        static_assert(false, "Template Argument must be a function type");
    };

    // size: 64
    template <typename Ret, typename... Args>
    class copyable_function<Ret(Args...)> {
    public:
        void *functor;
        union alignas(max_align_t) {
            std::byte dummy[detail::max_small_obj_size];
            void     *heap;
        };

        copyable_function() = delete;
    };

} // namespace brstd