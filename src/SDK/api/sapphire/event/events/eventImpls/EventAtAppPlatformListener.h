#pragma once

#include "SDK/api/src-deps/Application/AppPlatform.h"

namespace sapphire::event::impl {

    class SapphireEventAppPlatformListener : public AppPlatformListener {
        Bedrock::NonOwnerPointer<AppPlatform> mPlatform;

    public:
        SapphireEventAppPlatformListener();

        virtual ~SapphireEventAppPlatformListener() = default;

        virtual void onAppTerminated();
    };

} // namespace sapphire::event::impl