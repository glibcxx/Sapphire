#pragma once

#include "SDK/core/ApiManager.h"

enum class GameControllerButtonState : int32_t {
    Up = 0,
    Down = 1,
}; // GameControllerButtonState

enum class GameControllerErrorType : uint8_t {
    NoError = 0,
    ControllerRequired = 1,
    InvalidConfiguration = 2,
    AdhocOverWirelessConnectionLimit = 3,
    AdhocInvalidConfiguration = 4,
};

// size: 248
class GameControllerHandler {
public:
    using ButtonMap = std::unordered_map<uint32_t, int>;
    using ButtonStateMap = std::unordered_map<uint32_t, GameControllerButtonState>;

    ButtonMap                   mButtonMap;             // off+8
    std::vector<uint32_t>       mUnk72;                 // off+72
    std::vector<uint32_t>       mUnk96;                 // off+96
    std::vector<uint32_t>       mUnk120;                // off+120
    size_t                      mUnk144;                // off+144
    std::vector<uint32_t>       mUnk152;                // off+152
    size_t                      mUnk176;                // off+176
    std::vector<ButtonStateMap> mButtonState;           // off+184
    uint32_t                    mInputProcessResult[4]; // off+208
    std::thread                 mPollingThread;         // off+224
    std::atomic<bool>           mJoinThreads;           // off+240

    GameControllerHandler *ctor(); // \x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x8B\xF1\x48\x89\x4D\x00\x45\x33\xED(1.21.2)
                                   // \x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\xF9\x48\x89\x4C\x24\x00\x48\x8D\x05 (1.21.50)

    // off+0
    virtual ~GameControllerHandler();

    // vtb+1
    virtual void refresh(bool isInGame, bool isSplitScreen);

    // vtb+2
    virtual void refresh() = 0;

    // vtb+3
    virtual void shutdown();

    // vtb+4
    virtual GameControllerErrorType checkPlatformSpecificControllerError();

    // vtb+5
    virtual float normalizeAxis(float raw, float deadzone);

    // vtb+6
    virtual void normalizeAxes(float &, float &, float) = 0;
};
static_assert(sizeof(GameControllerHandler) == 248);