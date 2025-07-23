#pragma once

#include "IAppPlatform.h"
#include "SDK/core/ApiManager.h"
#include "SDK/api/src/common/AppPlatformListener.h"
#include "SDK/api/src-deps/Core/SecureStorage/SecureStorageKey.h"
#include "SDK/api/src-deps/Core/Utility/ServiceLocator.h"

class AppPlatform : public IAppPlatform, public ISecureStorageKeySystem {
public:
    SDK_API /*virtual*/ void initialize();

    SDK_API /*virtual*/ void addListener(AppPlatformListener *l, float priority);
};

template <>
SDK_API inline ServiceReference<AppPlatform> ServiceLocator<AppPlatform>::get();