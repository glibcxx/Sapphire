#pragma once

#include "SDK/core/ApiManager.h"
#include "UIScreenContext.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/MeshContext.h"
#include "SDK/api/src-deps/Core/Memory/LinearAllocator.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src-deps/MinecraftRenderer/Objects/FrameRenderObject.h"

namespace mce {
    class RenderDevice;
    class RendererSettings;
    class FrameBufferObject;
    class ViewportInfo;
    class Clock;
}; // namespace mce
class GuiData;
class Tessellator;
class MinecraftGraphicsPipeline;
class MinecraftGraphics;
class UIProfanityContext;
class CommandListQueue;

#if MC_VERSION == v1_21_2
// size: 264
class ScreenContext : public UIScreenContext,
                      public mce::MeshContext /*off+16*/ {
public:
    mce::RenderDevice                                 &renderDevice;              // off+136
    mce::RendererSettings                             &rendererSettings;          // off+144
    mce::FrameBufferObject                            *frameBufferObject;         // off+152
    const mce::ViewportInfo                           &viewport;                  // off+160
    Bedrock::NotNullNonOwnerPtr<const GuiData>         guiData;                   // off+168
    const mce::Clock                                  &clock;                     // off+184
    Tessellator                                       &tessellator;               // off+192
    MinecraftGraphicsPipeline                         &minecraftGraphicsPipeline; // off+200
    Bedrock::NonOwnerPointer<MinecraftGraphics>        minecraftGraphics;         // off+208
    Bedrock::NonOwnerPointer<const UIProfanityContext> uiProfanityContext;        // off+224
    void                                              *mUnk240;                   // off+240
    CommandListQueue                                  &commandListQueue;          // off+248
    LinearAllocator<FrameRenderObject>                &frameAllocator;            // off+256
};
static_assert(sizeof(ScreenContext) == 264);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
// size: 288
class ScreenContext : public UIScreenContext,
                      public mce::MeshContext /*off+16*/ {
public:
    mce::RenderDevice                                 &renderDevice;              // off+136
    mce::RendererSettings                             &rendererSettings;          // off+144
    mce::FrameBufferObject                            *frameBufferObject;         // off+152
    const mce::ViewportInfo                           &viewport;                  // off+160
    Bedrock::NotNullNonOwnerPtr<const GuiData>         guiData;                   // off+168
    const mce::Clock                                  &clock;                     // off+192
    Tessellator                                       &tessellator;               // off+200
    MinecraftGraphicsPipeline                         &minecraftGraphicsPipeline; // off+208
    Bedrock::NonOwnerPointer<MinecraftGraphics>        minecraftGraphics;         // off+216
    Bedrock::NonOwnerPointer<const UIProfanityContext> uiProfanityContext;        // off+240
    void                                              *mUnk264;                   // off+264
    CommandListQueue                                  &commandListQueue;          // off+272
    LinearAllocator<FrameRenderObject>                &frameAllocator;            // off+280
};
static_assert(sizeof(ScreenContext) == 288);
#endif
