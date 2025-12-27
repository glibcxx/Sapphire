#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "macros/Macros.h"

class FontRepository;
class Font;

// size: 80
class FontHandle : public Bedrock::EnableNonOwnerReferences {
public:
    Bedrock::NonOwnerPointer<FontRepository> mUnk24;         // off+24
    std::shared_ptr<Font>                    mUnk48;         // off+48
    uint64_t                                 mFontId;        // off+64
    bool                                     mIsDummyHandle; // off+72

    // vtb+0
    SDK_API virtual ~FontHandle();
#pragma SPHR_LINKER_SYM_ALIAS("??1FontHandle@@UEAA@XZ", "?dtor@FontHandle@@QEAAXXZ")

    SPHR_DECL_API("1.21.2", "\x40\x53\x48\x83\xEC\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8B\xD9\x48\x8D\x15")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8B\xD9\x48\x8D\x15")
    SDK_API void dtor() noexcept;
};