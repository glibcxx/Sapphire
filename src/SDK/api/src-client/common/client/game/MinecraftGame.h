#pragma once

#include "SDK/core/ApiManager.h"
#include "IMinecraftGame.h"
#include "SDK/api/src/common/App.h"
#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src/common/util/Timer.h"
#include "SDK/api/src/common/resources/ResourcePackListener.h"
#include "SDK/api/src/common/certificates/edu/CredentialsObserver.h"
#include "SDK/api/src-client/common/client/options/OptionsObserver.h"
#include "SDK/api/src-client/common/client/game/IDynamicPackageConsumer.h"
#include "SDK/api/src/common/locale/I18nObserver.h"

class IClientInstance;

#if MC_VERSION == v1_21_2
// size: 16
class Unk4CB6580 : public Core::Observer<Unk4CB6580, Core::SingleThreadedLock> {
    // vtb+0
    virtual ~Unk4CB6580();

    // vtb+2
    virtual void unk_virtual_2(__int64 a1, __int64 a2);
};
#endif

// size: 5512 (1.21.2), 5424 (1.21.50), 5248 (1.21.60)
class MinecraftGame : public IMinecraftGame,
                      public App,
                      public LevelListener,
                      public ResourcePackListener,
                      public OptionsObserver,
                      public edu::auth::CredentialsObserver,
#if MC_VERSION <= v1_21_50
                      public IDynamicPackageConsumer,
#endif
#if MC_VERSION == v1_21_2
                      public Unk4CB6580,
#endif
                      public I18nObserver {
public:
    // vtb+0
    virtual ~MinecraftGame() override;

    // vtb+2
    SDK_API virtual void update() override;

    SDK_API void startFrame();

    SDK_API void endFrame();

    SDK_API void updateGraphics(IClientInstance &client, const Timer &timer);

    SDK_API void updateAudio(IClientInstance &client, const Timer &timer);

    SDK_API void _update();

    SDK_API bool _clientUpdate();
};