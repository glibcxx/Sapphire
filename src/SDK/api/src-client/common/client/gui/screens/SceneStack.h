#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src-deps/Core/Utility/pubsub/Subscription.h"
#include "SDK/api/src-deps/json/Value.h"
#include "interfaces/ISceneStack.h"

class TaskGroup;
class CachedScene;
class VisualTree;
class UIControlFactory;
class LayoutManager;
class AbstractScene;
class UIEventCoordinator;
class Options;
class SceneStackProxy;
struct TextCharEventData;
namespace ui {
    enum SceneType : uint32_t;
}
namespace OreUI {
    struct RouteAction;
}

// size: 12
struct ScreenThreshold {
    ui::SceneType mSceneType;                // off+0
    ui::SceneType mSceneTypeToExclude;       // off+4
    int           mThresholdNumberOfScreens; // off+8
};

// size: 32
struct CachedScene {
    std::unique_ptr<VisualTree>       mVisualTree;     // off+0
    std::shared_ptr<UIControlFactory> mControlFactory; // off+8
    std::unique_ptr<LayoutManager>    mLayoutManager;  // off+24
};

// size: 32
class CachedScenes {
    bool                                                mLowMemoryDevice;        // off+0
    std::unique_ptr<TaskGroup>                          mDestroyScreenTaskGroup; // off+8
    std::map<Json::Value, std::unique_ptr<CachedScene>> mCachedScene;            // off+16
};

// size: 520 (1.21.2), 528 (1.21.50/1.21.60)
class SceneStack : public ISceneStack {
public:
    // size: 16
    class SceneStackEvent {
    public:
        enum class EventType : int {
            Push = 0,
            Pop = 1,
            Pop_Range_Of_Type = 2,
            Flush = 3,
        };

        EventType mType; // off+8

        // vtb+0
        virtual ~SceneStackEvent();
    };

    std::vector<std::shared_ptr<AbstractScene>>                          mScreenStack;                 // off+24
    CachedScenes                                                        &mCachedScreens;               // off+48
    std::vector<std::unique_ptr<SceneStackEvent>>                        mStackEvents;                 // off+56
    std::vector<std::unique_ptr<SceneStackEvent>>                        mQueuedStackEvents;           // off+80
    std::vector<ScreenThreshold>                                         mScreenThresholds;            // off+104
    std::unique_ptr<TaskGroup>                                           mDestroyScreenTaskGroup;      // off+128
    int                                                                  mActiveStackSize;             // off+136
    int                                                                  mScheduledScreenPushCount;    // off+140
    int                                                                  mScheduledScreenPopCount;     // off+144
    bool                                                                 mScreenNeedsEntrance;         // off+148
    std::string                                                          mLastPoppedScreenName;        // off+152
    bool                                                                 mChangedThisFrame;            // off+184
    bool                                                                 mScreenIsTicking;             // off+185
    bool                                                                 mReloadScenesOnNextPop;       // off+186
    bool                                                                 mDeferUpdatesUntilNextTick;   // off+187
    bool                                                                 mBufferTextCharEvents;        // off+188
    std::vector<TextCharEventData>                                       mBufferedTextCharEventData;   // off+192
    std::weak_ptr<Options>                                               mOptions;                     // off+216
    Bedrock::PubSub::Subscription                                        mAsyncLoadOptionSubscription; // off+232
    __int64                                                              _fill248[16];                 // off+248
    std::vector<std::pair<void *, std::function<void(AbstractScene &)>>> mSceneChangeCallbacks;        // off+376
    std::vector<std::pair<void *, std::function<void(AbstractScene &)>>> mPrePushSceneCallbacks;       // off+400
    std::vector<std::pair<void *, std::function<void(std::shared_ptr<AbstractScene>, std::optional<OreUI::RouteAction>)>>>
                                                                         mPushSceneCallbacks;   // off+424
    std::vector<std::pair<void *, std::function<void(AbstractScene &)>>> mPrePopSceneCallbacks; // off+448
    std::vector<std::pair<void *, std::function<void(std::shared_ptr<AbstractScene>, bool, std::optional<OreUI::RouteAction>)>>>
                                                    mPopSceneCallbacks;  // off+472
    std::unique_ptr<SceneStackProxy>                mProxy;              // off+496
    Bedrock::NotNullNonOwnerPtr<UIEventCoordinator> mUIEventCoordinator; // off+504

    SDK_API SceneStack(
        CachedScenes &cachedScenes, const Bedrock::NotNullNonOwnerPtr<UIEventCoordinator> &uiEventCoordinator
    );

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x54\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF0\x48\x8B\xDA")
    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF0\x4C\x8B\xF2\x48\x8B\xD9")
    SPHR_DECL_API("1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF0\x48\x8B\xDA\x48\x8B\xF9\x48\x89\x4C\x24")
    SPHR_CTOR_ALIAS SDK_API SceneStack *ctor(
        CachedScenes &cachedScenes, const Bedrock::NotNullNonOwnerPtr<UIEventCoordinator> &uiEventCoordinator
    );
};