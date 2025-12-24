#include "EventHooks.h"

#include "EventAtClientInstanceEventListener.h"
#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src-deps/Application/AppPlatform.h"
#include "SDK/api/sapphire/event/EventBus.h"
#include "../AppInitializedEvent.h"
#include "../FrameEvents.h"
#include "../ClientUpdateEvent.h"
#include "../MinecraftGameUpdateEvent.h"
#include "../GameUpdateGraphicEvent.h"
#include "../MinecraftGameInitFinishedEvent.h"

#include "EventAtAppPlatformListener.h"

namespace sapphire::event::impl {

    std::unique_ptr<SapphireEventAppPlatformListener>    gAppPlatformListener = nullptr;
    std::unique_ptr<SapphireClientInstanceEventListener> gClientInstanceEventListener = nullptr;

    HOOK_TYPE(
        AppPlatformHook,
        AppPlatform,
        sapphire::hook::HookPriority::Normal,
        AppPlatform::initialize,
        void
    ) {
        this->origin();
        EventBus::getInstance().dispatchEvent(AppInitializedEvent{*this});
        gAppPlatformListener = std::make_unique<SapphireEventAppPlatformListener>();
    }

    HOOK_TYPE(
        MinecraftGameInitCompleteHook,
        MinecraftGame,
        sapphire::HookPriority::Normal,
        MinecraftGame::_initFinish,
        SerialWorkList::WorkResult,
        std::shared_ptr<MinecraftGame::InitContext> &initContext
    ) {
        auto res = this->origin(initContext);
        if (res == SerialWorkList::WorkResult::Complete) {
            assert(ClientInstance::primaryClientInstance && "primaryClientInstance is not ready.");
            EventBus::getInstance().dispatchEvent(
                MinecraftGameInitFinishedEvent{*this, *ClientInstance::primaryClientInstance}
            );
            gClientInstanceEventListener =
                std::make_unique<SapphireClientInstanceEventListener>(*ClientInstance::primaryClientInstance);
        }
        return res;
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
        EventBus::getInstance().dispatchEvent(MinecraftGameStartFrameEvent{*this});
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
        EventBus::getInstance().dispatchEvent(MinecraftGameEndFrameEvent{*this});
    }

    HOOK_TYPE(
        FrameBuilderStartFrameHook,
        mce::framebuilder::BgfxFrameBuilder,
        sapphire::hook::HookPriority::Normal,
        mce::framebuilder::BgfxFrameBuilder::startFrame,
        void
    ) {
        EventBus::getInstance().dispatchEvent(FrameBuilderStartFrameEvent{*this});
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
        EventBus::getInstance().dispatchEvent(FrameBuilderEndFrameEvent{*this, frameBuilderContext});
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
        EventBus::getInstance().dispatchEvent(MinecraftGameUpdateEvent{*this});
        this->origin();
    }

    HOOK_TYPE(
        ClientUpdateHook,
        MinecraftGame,
        sapphire::hook::HookPriority::Normal,
        MinecraftGame::_clientUpdate,
        bool
    ) {
        EventBus::getInstance().dispatchEvent(ClientUpdateEvent{*this});
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
        EventBus::getInstance().dispatchEvent(GameUpdateGraphicEvent{*this, client, timer});
        this->origin(client, timer);
    }

    void EventHooks::init() {
        if (!AppPlatformHook::hook())
            sapphire::error("EventHooks: AppPlatformHook::hook failed!");
        if (!MinecraftGameInitCompleteHook::hook())
            sapphire::error("EventHooks: MinecraftGameInitCompleteHook::hook failed!");
        if (!MinecraftGameStartFrameHook::hook())
            sapphire::error("EventHooks: MinecraftGameStartFrameHook::hook failed!");
        if (!MinecraftGameEndFrameHook::hook())
            sapphire::error("EventHooks: MinecraftGameEndFrameHook::hook failed!");
        if (!FrameBuilderStartFrameHook::hook())
            sapphire::error("EventHooks: FrameBuilderStartFrameHook::hook failed!");
        if (!FrameBuilderEndFrameHook::hook())
            sapphire::error("EventHooks: FrameBuilderEndFrameHook::hook failed!");
        if (!MinecraftGameUpdateHook::hook())
            sapphire::error("EventHooks: MinecraftGameUpdateHook::hook failed!");
        if (!ClientUpdateHook::hook())
            sapphire::error("EventHooks: ClientUpdateHook::hook failed!");
        if (!GameUpdateGraphicHook::hook())
            sapphire::error("EventHooks: GameUpdateGraphicHook::hook failed!");
    }

    void EventHooks::uninit() {
        GameUpdateGraphicHook::unhook();
        ClientUpdateHook::unhook();
        MinecraftGameUpdateHook::unhook();
        FrameBuilderEndFrameHook::unhook();
        FrameBuilderStartFrameHook::unhook();
        MinecraftGameEndFrameHook::unhook();
        MinecraftGameStartFrameHook::unhook();
        MinecraftGameInitCompleteHook::unhook();
        AppPlatformHook::unhook();
    }

} // namespace sapphire::event::impl