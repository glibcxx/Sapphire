#pragma once

#include "IAppPlatform.h"
#include "SDK/core/ApiManager.h"
#include "SDK/api/src/common/AppPlatformListener.h"
#include "SDK/api/src-deps/Core/SecureStorage/SecureStorageKey.h"
#include "SDK/api/src-deps/Core/Utility/ServiceLocator.h"

class AppPlatform : public IAppPlatform, public ISecureStorageKeySystem {
public:
    SDK_API /*virtual*/ void initialize();
    MARK_HOOKABLE(&AppPlatform::initialize)

    SDK_API /*virtual*/ void addListener(AppPlatformListener *l, float priority);
    MARK_HOOKABLE(&AppPlatform::addListener)
};

template <>
SDK_API ServiceReference<AppPlatform> ServiceLocator<AppPlatform>::get();
MARK_HOOKABLE(&ServiceLocator<AppPlatform>::get)
