#pragma once

#include "RenderOrder.h"

class LevelRendererPlayer;

class PlayerRenderView {
public:
    // size: 8
    struct LegacyPlayerRenderPass {
        optional_ref<PlayerRenderView> mModule; // off+0
    };

    // size: 3
    struct PostInitInfo {
        bool mInitialized;                  // off+0
        bool mNeedResolveBeforePostProcess; // off+1
        bool mRequiresBackingTextures;      // off+2
    };

    rendergraph::APIResourcePool                           mAPIResourcePool;             // off+0
    rendergraph::RenderOrder                               mPlayerRenderOrder;           // off+176
    LevelRendererPlayer                                   &mPlayerView;                  // off+472
    optional_ref<PlayerRenderView::LegacyPlayerRenderPass> mLegacyPassRuntimeData;       // off+480
    PlayerRenderView::PostInitInfo                         mPostInitInfo;                // off+488

    // ...
};