#pragma once

#include <memory>

#include "IClientInstance.h"
#include "SDK/api/src/common/world/Minecraft.h"
#include "SDK/api/src-deps/Core/File/StorageAreaStateListener.h"
#include "SDK/api/src/common/world/actor/player/PlayerListener.h"
#include "SDK/api/src/common/entity/components/RenderCameraComponent.h"

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
        // \x40\x53\x48\x83\xEC\x00\x48\x8B\x01\x48\x8B\xD9\x48\x8B\x80\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x84\xC0\x74\x00\x48\x8B\x8B\x00\x00\x00\x00\x48\x8B\x01\x48\x8B\x80\x00\x00\x00\x00\xFF\x15
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        return memory::vCall<bool>(this, 167);
#elif MC_VERSION == v1_21_2
        return memory::vCall<bool>(this, 162);
#endif
    }

    bool isShowingMenu() const {
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        return memory::vCall<bool>(this, 172); // \x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xD9\x45\x33\xFF\x44\x89\x7C\x24\x00\x44\x88\x7D
#elif MC_VERSION == v1_21_2
        return memory::vCall<bool>(this, 167); // \x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xD9\x33\xFF\x89\x7C\x24\x00\x40\x88\x7D
#endif
    }

    void grabMouse() {
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        memory::vCall(this, 340);
#elif MC_VERSION == v1_21_2
        memory::vCall(this, 332);
#endif
    }

    void releaseMouse() {
        // \x40\x53\x48\x83\xEC\x00\x48\x8B\x01\x48\x8B\xD9\x48\x8B\x80\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x84\xC0\x74\x00\x48\x8B\x8B\x00\x00\x00\x00\x48\x8B\x01\x48\x8B\x80\x00\x00\x00\x00\x48\x83\xC4\x00\x5B\x48\xFF\x25\x00\x00\x00\x00\x48\x83\xC4\x00\x5B\xC3\x48\x89\x5C\x24
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        memory::vCall(this, 341);
#elif MC_VERSION == v1_21_2
        memory::vCall(this, 333);
#endif
    }

    RenderCameraComponent *getRenderCameraComponent();

    SDK_API static void *const *__vftable0;
    SDK_API static void *const *__vftable1;
    SDK_API static void *const *__vftable2;
    SDK_API static void *const *__vftable3;

    SDK_API static ClientInstance *primaryClientInstance;
};