#pragma once

#include <memory>

#include "IClientInstance.h"
#include "SDK/api/src/common/world/Minecraft.h"
#include "SDK/api/src-deps/Core/File/StorageAreaStateListener.h"
#include "SDK/api/src/common/world/actor/player/PlayerListener.h"

class ClientInstance : public IClientInstance,
                       public ::Core::StorageAreaStateListener,
                       public GameCallbacks,
                       public PlayerListener,
                       public std::enable_shared_from_this<ClientInstance> {
public:
    static_assert(sizeof(IClientInstance) == 24);
    static_assert(sizeof(::Core::StorageAreaStateListener) == 128 - 24);
    static_assert(sizeof(PlayerListener) == 136 - 128);

    bool getMouseGrabbed() const {
        memory::vCall(this, 167);
    }

    bool isShowingMenu() const {
        return memory::vCall<bool>(this, 172);
    }

    void grabMouse() {
        memory::vCall(this, 340);
    }

    void releaseMouse() {
        memory::vCall(this, 341);
    }

    SDK_API static void *const *__vftable0;
    SDK_API static void *const *__vftable1;
    SDK_API static void *const *__vftable2;
    SDK_API static void *const *__vftable3;

    SDK_API static ClientInstance *primaryClientInstance;
};