#pragma once

#include "Subject.h"

namespace Core {

    // size: 16
    template <typename TObserver, typename Lock>
    class Observer {
    public:
        using SubjectType = Subject<TObserver, Lock>;

        // vtb+0
        virtual ~Observer();

        SubjectType *mpSubject; // off+8
    };

} // namespace Core