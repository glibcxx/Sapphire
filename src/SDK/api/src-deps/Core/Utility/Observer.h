#pragma once

#include "Subject.h"

namespace Core {

    // size: 1
    class SingleThreadedLock {
    public:
        void lock();

        void unlock();
    };

    // size: 16
    template <typename TObserver, typename Lock>
    class Observer {
    public:
        using SubjectType = Subject<TObserver, Lock>;

        SubjectType *mpSubject; // off+8

        // vtb+0
        virtual ~Observer();

        // vtb+1
        virtual void _onSubjectDestroyed();
    };

} // namespace Core