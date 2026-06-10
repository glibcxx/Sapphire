#pragma once

#include "pch.h" // IWYU pragma: keep

namespace Core {

    template <typename TObserver, typename Lock>
    class Subject {
    public:
        using Type = TObserver;

        Lock              mLock;
        std::vector<Type> mObservers;
    };

} // namespace Core