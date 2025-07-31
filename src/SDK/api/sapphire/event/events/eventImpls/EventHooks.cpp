#include "EventHooks.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/src-deps/Application/AppPlatform.h"
#include "SDK/api/sapphire/event/EventManager.h"
#include "../AppInitializedEvent.h"
#include "../FrameEvents.h"
#include "../ClientUpdateEvent.h"
#include "../MinecraftGameUpdateEvent.h"
#include "../GameUpdateGraphicEvent.h"

#include "EventAtAppPlatformListener.h"

namespace sapphire::event {

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

    /**
     * vvvv Frame Events vvvv
     */

    HOOK_TYPE(
        MinecraftGameStartFrameHook,
        MinecraftGame,
        sapphire::hook::HookPriority::Normal,
        MinecraftGame::startFrame,
        void
    ) {
        EventManager::getInstance().dispatchEvent(MinecraftGameStartFrameEvent{*this});
        this->origin();
    }

    HOOK_TYPE(
        MinecraftGameEndFrameHook,
        MinecraftGame,
        sapphire::hook::HookPriority::Normal,
        MinecraftGame::endFrame,
        void
    ) {
        this->origin();
        EventManager::getInstance().dispatchEvent(MinecraftGameEndFrameEvent{*this});
    }

    HOOK_TYPE(
        FrameBuilderStartFrameHook,
        mce::framebuilder::BgfxFrameBuilder,
        sapphire::hook::HookPriority::Normal,
        mce::framebuilder::BgfxFrameBuilder::startFrame,
        void
    ) {
        EventManager::getInstance().dispatchEvent(FrameBuilderStartFrameEvent{*this});
        this->origin();
    }

    HOOK_TYPE(
        FrameBuilderEndFrameHook,
        mce::framebuilder::BgfxFrameBuilder,
        sapphire::hook::HookPriority::Normal,
        mce::framebuilder::BgfxFrameBuilder::endFrame,
        void,
        mce::framebuilder::FrameBuilderContext &&frameBuilderContext
    ) {
        this->origin(std::move(frameBuilderContext));
        EventManager::getInstance().dispatchEvent(FrameBuilderEndFrameEvent{*this, frameBuilderContext});
    }

    /**
     * vvvv Game Update Events vvvv
     */

    HOOK_TYPE(
        MinecraftGameUpdateHook,
        MinecraftGame,
        sapphire::hook::HookPriority::Normal,
        MinecraftGame::_update,
        void
    ) {
        EventManager::getInstance().dispatchEvent(MinecraftGameUpdateEvent{*this});
        this->origin();
    }

    HOOK_TYPE(
        ClientUpdateHook,
        MinecraftGame,
        sapphire::hook::HookPriority::Normal,
        MinecraftGame::_clientUpdate,
        bool
    ) {
        EventManager::getInstance().dispatchEvent(ClientUpdateEvent{*this});
        return this->origin();
    }

    HOOK_TYPE(
        GameUpdateGraphicHook,
        MinecraftGame,
        sapphire::hook::HookPriority::Normal,
        MinecraftGame::updateGraphics,
        void,
        IClientInstance &client,
        const Timer     &timer
    ) {
        EventManager::getInstance().dispatchEvent(GameUpdateGraphicEvent{*this, client, timer});
        this->origin(client, timer);
    }

    void EventHooks::init() {
        if (!AppPlatformHook::hook())
            Logger::Error("[EventHooks] AppPlatformHook::hook failed!");
        if (!MinecraftGameStartFrameHook::hook())
            Logger::Error("[EventHooks] MinecraftGameStartFrameHook::hook failed!");
        if (!MinecraftGameEndFrameHook::hook())
            Logger::Error("[EventHooks] MinecraftGameEndFrameHook::hook failed!");
        if (!FrameBuilderStartFrameHook::hook())
            Logger::Error("[EventHooks] FrameBuilderStartFrameHook::hook failed!");
        if (!FrameBuilderEndFrameHook::hook())
            Logger::Error("[EventHooks] FrameBuilderEndFrameHook::hook failed!");
        if (!MinecraftGameUpdateHook::hook())
            Logger::Error("[EventHooks] MinecraftGameUpdateHook::hook failed!");
        if (!ClientUpdateHook::hook())
            Logger::Error("[EventHooks] ClientUpdateHook::hook failed!");
        if (!GameUpdateGraphicHook::hook())
            Logger::Error("[EventHooks] GameUpdateGraphicHook::hook failed!");
    }

    void EventHooks::uninit() {
        GameUpdateGraphicHook::unhook();
        ClientUpdateHook::unhook();
        MinecraftGameUpdateHook::unhook();
        FrameBuilderEndFrameHook::unhook();
        FrameBuilderStartFrameHook::unhook();
        MinecraftGameEndFrameHook::unhook();
        MinecraftGameStartFrameHook::unhook();
        AppPlatformHook::unhook();
    }

} // namespace sapphire::event