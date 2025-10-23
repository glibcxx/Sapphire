#pragma once

#include "SDK/api/src-deps/Core/Utility/Observer.h"

namespace edu::auth {

    // size: 16
    struct CredentialsObserver : public Core::Observer<CredentialsObserver, Core::SingleThreadedLock> {
    public:
        // vtb+0
        virtual ~CredentialsObserver() override;

        // ...
    };

} // namespace edu::auth