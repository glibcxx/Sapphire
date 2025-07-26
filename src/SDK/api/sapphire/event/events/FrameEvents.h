#pragma once

#include "../Event.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"
#include "SDK/api/src-deps/MinecraftRenderer/FrameBuilder/BgfxFrameBuilder.h"

namespace sapphire::event {

    class MinecraftGameStartFrameEvent : public Event {
    public:
        MinecraftGame &mMinecraftGame;

        MinecraftGameStartFrameEvent(MinecraftGame &minecraftGame) :
            mMinecraftGame(minecraftGame) {}
    };

    class MinecraftGameEndFrameEvent : public Event {
    public:
        MinecraftGame &mMinecraftGame;

        MinecraftGameEndFrameEvent(MinecraftGame &minecraftGame) :
            mMinecraftGame(minecraftGame) {}
    };

    class FrameBuilderStartFrameEvent : public Event {
    public:
        mce::framebuilder::BgfxFrameBuilder &mFrameBuilder;

        FrameBuilderStartFrameEvent(mce::framebuilder::BgfxFrameBuilder &frameBuilder) :
            mFrameBuilder(frameBuilder) {}
    };

    class FrameBuilderEndFrameEvent : public Event {
    public:
        mce::framebuilder::BgfxFrameBuilder    &mFrameBuilder;
        mce::framebuilder::FrameBuilderContext &mFrameBuilderContext;

        FrameBuilderEndFrameEvent(
            mce::framebuilder::BgfxFrameBuilder    &frameBuilder,
            mce::framebuilder::FrameBuilderContext &frameBuilderContext
        ) :
            mFrameBuilder(frameBuilder), mFrameBuilderContext(frameBuilderContext) {}
    };

} // namespace sapphire::event