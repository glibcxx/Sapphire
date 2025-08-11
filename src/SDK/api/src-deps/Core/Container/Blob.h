#pragma once

#include <memory>

namespace mce {

    // size: 24
    class Blob {
    public:
        using value_type = uint8_t;
        using delete_function = void (*)(value_type *);

        struct Deleter // size: 8
        {
            void operator()(value_type *p) const {
                if (p && m_fun)
                    m_fun(p);
            }

            delete_function m_fun; // off+0
        };

        using pointer_type = std::unique_ptr<value_type[], Deleter>;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using iterator = value_type *;
        using const_iterator = const value_type *;
        using difference_type = ptrdiff_t;
        using size_type = size_t;

        pointer_type mBlob; // off+0
        size_type    mSize; // off+16
    };

} // namespace mce