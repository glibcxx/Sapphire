#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Coregraphics/ImageBuffer.h"

// size: 80 (1.21.50)
class ScreenshotRecorder : public Bedrock::EnableNonOwnerReferences {
public:
    bool            mEnabled;        // off+24
    cg::ImageBuffer mLastBackBuffer; // off+32

    // vtb+0
    virtual ~ScreenshotRecorder() override;
};