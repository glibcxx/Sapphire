#pragma once

#include <type_traits>

namespace sapphire {

    template <typename TBase, typename TImpl>
    class DowncastImpl {
    protected:
        DowncastImpl() = default;
        ~DowncastImpl() = default;

        TImpl *self() {
            static_assert(std::is_base_of_v<TBase, TImpl>, "Derived class must inherit from TBase");
            return static_cast<TImpl *>(this);
        }

        const TImpl *self() const {
            return static_cast<const TImpl *>(this);
        }
    };

} // namespace sapphire