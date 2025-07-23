#include "EventHooks.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/src-deps/Application/AppPlatform.h"
#include "SDK/api/sapphire/event/EventManager.h"
#include "../AppInitializedEvent.h"

#include "EventAtAppPlatformListener.h"

std::unique_ptr<SapphireEventAppPlatformListener> gAppPlatformListener = nullptr;

HOOK_TYPE(
    AppPlatformHook,
    AppPlatform,
    sapphire::hook::HookPriority::Normal,
    AppPlatform::initialize,
    void
) {
    this->origin();
    EventManager::getInstance().dispatchEvent(AppInitializedEvent{*this});
    gAppPlatformListener = std::make_unique<SapphireEventAppPlatformListener>();
}

void EventHooks::init() {
    if (!AppPlatformHook::hook())
        Logger::Error("[EventHooks] AppPlatformHook::hook failed!");
    else
        Logger::Debug("[EventHooks] AppPlatformHook::hook success!");
}

void EventHooks::uninit() {
    AppPlatformHook::unhook();
}
