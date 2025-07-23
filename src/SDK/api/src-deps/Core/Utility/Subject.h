#pragma once

#include <vector>

namespace Core {

    template <typename TObserver, typename Lock>
    class Subject {
    public:
        using Type = TObserver;

        Lock              mLock;
        std::vector<Type> mObservers;
    };

} // namespace Core