#include "ChunkBorderRenderer.h"

#include "SDK/api/src/common/world/level/ChunkPos.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-client/common/client/renderer/game/LevelRendererPlayer.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/sapphire/event/events/RenderLevelEvent.h"
#include "SDK/api/sapphire/event/EventBus.h"
#include "SDK/api/sapphire/util/DrawUtils.h"
#include "SDK/api/sapphire/GUI/GUI.h"

static sapphire::event::AutoListener<sapphire::event::RenderLevelEvent> renderListener;

static DrawUtils *drawUtils = nullptr;
static bool       drawBorder = false;

void onRenderLevel(sapphire::event::RenderLevelEvent &event) {
    if (!drawBorder) return;
    const Vec3 &camPos = event.mLevelRenderer.getLevelRendererPlayer().getCameraPos();
    ChunkPos    chunkPos{(int)std::floor(camPos.x) >> 4, (int)std::floor(camPos.z) >> 4};

    for (int cx = -1; cx <= 2; ++cx) {
        for (int cz = -1; cz <= 2; ++cz) {
            Vec3 vLineFrom(chunkPos.x + cx << 4, camPos.y - 128, chunkPos.z + cz << 4);
            Vec3 vLineTo(chunkPos.x + cx << 4, camPos.y + 128, chunkPos.z + cz << 4);
            // 绘制4区块交界处的竖线
            drawUtils->drawLine(
                vLineFrom,
                vLineTo,
                cx != -1 && cx != 2 && cz != -1 && cz != 2 ? mce::Colors::BLUE : mce::Colors::RED
            );
        }
    }

    // 绘制中心区块的边界竖线
    Vec3 vLineFrom(chunkPos.x << 4, camPos.y - 128, chunkPos.z << 4);
    Vec3 vLineTo(chunkPos.x << 4, camPos.y + 128, chunkPos.z << 4);
    for (int i = 2; i < 16; i += 2) {
        drawUtils->drawLine(
            vLineFrom + Vec3{(float)i, 0, 0},
            vLineTo + Vec3{(float)i, 0, 0},
            i % 4 == 0 ? mce::Colors::CYAN : mce::Colors::YELLOW
        );
        drawUtils->drawLine(
            vLineFrom + Vec3{0, 0, (float)i},
            vLineTo + Vec3{0, 0, (float)i},
            i % 4 == 0 ? mce::Colors::CYAN : mce::Colors::YELLOW
        );
        drawUtils->drawLine(
            vLineFrom + Vec3{16, 0, (float)i},
            vLineTo + Vec3{16, 0, (float)i},
            i % 4 == 0 ? mce::Colors::CYAN : mce::Colors::YELLOW
        );
        drawUtils->drawLine(
            vLineFrom + Vec3{(float)i, 0, 16},
            vLineTo + Vec3{(float)i, 0, 16},
            i % 4 == 0 ? mce::Colors::CYAN : mce::Colors::YELLOW
        );
    }

    Vec3 hLineFrom(chunkPos.x << 4, 0, chunkPos.z << 4);
    for (int y = (int)(camPos.y - 128) / 2 * 2; y < camPos.y + 128; y += 2) {
        const mce::Color *color = y % 16 == 0 ? &mce::Colors::BLUE
                                : y % 8 == 0  ? &mce::Colors::CYAN
                                              : &mce::Colors::YELLOW;
        drawUtils->drawLine(
            hLineFrom + Vec3{0, (float)y, 0},
            hLineFrom + Vec3{16, (float)y, 0},
            *color
        );
        drawUtils->drawLine(
            hLineFrom + Vec3{0, (float)y, 0},
            hLineFrom + Vec3{0, (float)y, 16},
            *color
        );
        drawUtils->drawLine(
            hLineFrom + Vec3{16, (float)y, 0},
            hLineFrom + Vec3{16, (float)y, 16},
            *color
        );
        drawUtils->drawLine(
            hLineFrom + Vec3{0, (float)y, 16},
            hLineFrom + Vec3{16, (float)y, 16},
            *color
        );
    }
}

void setupSettings() {
    GuiOverlay::registerHotkey(
        {.keysDown = {ImGuiKey_F3},
         .triggerKey = ImGuiKey_G,
         .action = []() {
             drawBorder = !drawBorder;
             GuiOverlay::addToast(std::format("ChunkBorder: {}", drawBorder ? "Enabled" : "Disabled"));
         }}
    );
    GuiOverlay::registerPluginSettings(
        {
            .name = "Chunk Border",
            .description = "Display Chunk Border",
            .drawSettings = []() {
                ImGui::Checkbox("Draw Chunk Border [F3+G]", &drawBorder);
            },
        }
    );
}

void installChunkBorderRender() {
    drawUtils = &DrawUtils::getInstance();
    renderListener = sapphire::event::EventBus::getInstance()
                         .registerAutoListener<sapphire::event::RenderLevelEvent>(onRenderLevel);
    setupSettings();
}