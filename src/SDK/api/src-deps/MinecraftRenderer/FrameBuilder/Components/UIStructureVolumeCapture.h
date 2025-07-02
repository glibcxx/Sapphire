#pragma once

#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"

namespace MFC /*mce::framebuilder::gamecomponents*/ {

    // size: 72
    struct UIStructureVolumeCapture {
        mce::ServerResourcePointer<void> mUnk0;     // off+0
        mce::ServerResourcePointer<void> mUnk24;    // off+24
        uintptr_t                        mUnk48[3]; // off+48
    };
    static_assert(sizeof(UIStructureVolumeCapture) == 72);

} // namespace MFC