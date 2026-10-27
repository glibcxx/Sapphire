#pragma once

namespace mce::AlignmentHelper {

    template <typename Val, size_t Align>
    class AlignmentAllocator {
    public:
        using value_type = Val;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;

        AlignmentAllocator() noexcept = default;
        ~AlignmentAllocator() noexcept = default;

        template <typename U>
        AlignmentAllocator(const AlignmentAllocator<U, Align> &) noexcept {}

        const_pointer adress(const_reference r) const noexcept { return std::addressof(r); }

        pointer adress(reference r) noexcept { return std::addressof(r); }

        pointer allocate(size_type n) { return static_cast<pointer>(::_aligned_malloc(n * sizeof(value_type), Align)); }

        void deallocate(pointer p, size_type) noexcept { ::_aligned_free(p); }

        void construct(pointer p, const_reference r) { ::new (static_cast<void *>(p)) value_type(r); }

        void destroy(pointer p) { p->~value_type(); }

        size_type max_size() const noexcept { return (std::numeric_limits<size_type>::max)() / sizeof(Val); }

        bool operator!=(const AlignmentAllocator &) const { return false; }
        bool operator==(const AlignmentAllocator &) const { return true; }

        template <typename T>
        struct rebind {
        public:
            using other = AlignmentAllocator<T, Align>;
        };
    };

} // namespace mce::AlignmentHelper
