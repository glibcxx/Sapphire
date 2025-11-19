#pragma once

#include "SDK/api/src-deps/Core/Threading/Async.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src-deps/Core/Utility/pubsub/Subscription.h"
#include "SDK/api/src/common/ActorUniqueID.h"
#include "SDK/api/src/common/world/level/BlockPos.h"

class IMinecraftGame;
class IClientInstance;
class IAdvancedGraphicsOptions;
class TaskGroup;
class UISoundPlayer;
class CachedScenes;
class UIControlFactory;
class VisualTree;
class LayoutManager;
class UIControl;
class SceneFactoryProxy;
class UIScene;
class Player;
namespace OreUI {
    class SceneProvider;
}

// size: 216 (1.21.2), 232 (1.21.50/1.21.60)
class SceneFactory {
public:
    // size: 96
    struct PreCachePackage {
        std::string                                    screenName;     // off+0
        std::shared_ptr<UIControlFactory>              controlFactory; // off+32
        std::unique_ptr<VisualTree>                    visualTree;     // off+48
        std::unique_ptr<LayoutManager>                 layoutManager;  // off+56
        std::shared_ptr<UIControl>                     rootControl;    // off+64
        Bedrock::Threading::IAsyncResult<void>::Handle taskHandle;     // off+80
    };

    Bedrock::NotNullNonOwnerPtr<IClientInstance>          mClient;                 // off+8
    IMinecraftGame                                       &mMinecraft;              // off+32
    Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsOptions> mGraphicsOptions;        // off+40
    bool                                                  mUnk64;                  // off+64
    std::unique_ptr<TaskGroup>                            mTaskGroup;              // off+72
    bool                                                  mUseClientInstanceStack; // off+80
    OreUI::SceneProvider                                 &mSceneProvider;          // off+88
    std::unique_ptr<UISoundPlayer>                        mSoundPlayer;            // off+96
    CachedScenes                                         &mCachedScenes;           // off+104

    std::vector<std::shared_ptr<PreCachePackage>> mPreCachePackages; // off+112
    bool                                          mUnk136;           // off+136
    std::unique_ptr<SceneFactoryProxy>            mProxy;            // off+144
    std::unordered_map<
        HashedString,
        std::pair<
            std::string,
            std::function<
                std::shared_ptr<UIScene>(
                    SceneFactory &,
                    IMinecraftGame &,
                    IClientInstance &,
                    const Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsOptions> &,
                    const std::string &,
                    Player &,
                    const BlockPos &,
                    ActorUniqueID
                )>>>
                                  mRegisteredInGameScreens; // off+152
    Bedrock::PubSub::Subscription mUnk216;                  // off+216

    SDK_API SceneFactory(
        IMinecraftGame                                              &minecraft,
        const Bedrock::NotNullNonOwnerPtr<IClientInstance>          &client,
        const Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsOptions> &graphicsOptions,
        OreUI::SceneProvider                                        &sceneProvider
    );
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                                                                                                                                    \
    "??0SceneFactory@@QEAA@AEAVIMinecraftGame@@AEBV?$not_null@V?$NonOwnerPointer@VIClientInstance@@@Bedrock@@@gsl@@AEBV?$not_null@V?$NonOwnerPointer@VIAdvancedGraphicsOptions@@@Bedrock@@@3@AEAVSceneProvider@OreUI@@@Z",        \
    "?ctor@SceneFactory@@QEAAPEAV1@AEAVIMinecraftGame@@AEBV?$not_null@V?$NonOwnerPointer@VIClientInstance@@@Bedrock@@@gsl@@AEBV?$not_null@V?$NonOwnerPointer@VIAdvancedGraphicsOptions@@@Bedrock@@@4@AEAVSceneProvider@OreUI@@@Z" \
)

    SDK_API SceneFactory *ctor(
        IMinecraftGame                                              &minecraft,
        const Bedrock::NotNullNonOwnerPtr<IClientInstance>          &client,
        const Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsOptions> &graphicsOptions,
        OreUI::SceneProvider                                        &sceneProvider
    );

    // vtb+0
    SDK_API virtual ~SceneFactory();
#pragma SPHR_LINKER_SYM_ALIAS("??1SceneFactory@@UEAA@XZ", "?dtor@SceneFactory@@QEAAXXZ")

    SDK_API void dtor() noexcept;
};