#pragma once

#include "ActorShaderManager.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-deps/MinecraftRenderer/Objects/NameTagRenderObjectCollection.h"

class Tessellator;
class Font;
struct ViewRenderData;

// size: 120
class BaseActorRenderer : public ActorShaderManager {
public:
    mce::MaterialPtr mNameTagMat;             // off+56
    mce::MaterialPtr mDepthTestedNameTagMat;  // off+72
    mce::MaterialPtr mDepthTestedNameTextMat; // off+88
    mce::MaterialPtr mDepthTestedHealthMat;   // off+104

    SAPPHIRE_API("1.21.2", "mov", "\x0F\x10\x05\x00\x00\x00\x00\x0F\x28\x25")
    SAPPHIRE_API("1.21.50,1.21.60", "mov", "\x0F\x10\x1D\x00\x00\x00\x00\x0F\x29\x5D")
    SDK_API static const mce::Color *NAME_TAG_COLOR;

    // vtb+0
    virtual ~BaseActorRenderer() = default;

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x83\xC4\x00\x5B\xC3\x4C\x89\x53")
    SDK_API std::vector<NameTagRenderObject> extractRenderTextObjects(
        Tessellator            &tessellator,
        const std::string      &str,
        const std::vector<int> &widths,
        const Vec3             &pos,
        const mce::Color       &color,
        bool                    realityFullVRMode
    );

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x83\xC3\x70\x48\x3B\x00\x75\xD3")
    SDK_API static void renderText(
        ScreenContext             &screenContext,
        const ViewRenderData      &viewData,
        const NameTagRenderObject &tagData,
        Font                      &font,
        float                      size
    );
};