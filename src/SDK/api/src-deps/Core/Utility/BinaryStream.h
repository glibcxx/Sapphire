#pragma once

#include "SDK/api/src/common/platform/Result.h"

// size: 64 (1.21.2), 72 (1.21.50/1.21.60)
class ReadOnlyBinaryStream {
public:
#if MC_VERSION <= v1_21_2
    size_t             mReadPointer = 0;       // off+8
    bool               mHasOverflowed = false; // off+16
    const std::string  mOwnedBuffer;           // off+24
    const std::string &mBuffer;                // off+56
#elif MC_VERSION == v1_21_50 || v1_21_60
    std::string      mOwnedBuffer;           // off+8
    std::string_view mView;                  // off+40
    size_t           mReadPointer = 0;       // off+56
    bool             mHasOverflowed = false; // off+64
#endif

#if MC_VERSION <= v1_21_2
    ReadOnlyBinaryStream(std::string &buffer, bool copyBuffer) :
        mOwnedBuffer(buffer), mBuffer(copyBuffer ? mOwnedBuffer : "") {}
#elif MC_VERSION == v1_21_50 || v1_21_60
    ReadOnlyBinaryStream(std::string &buffer, bool copyBuffer) :
        mOwnedBuffer(buffer), mView(copyBuffer ? std::string_view{mOwnedBuffer} : std::string_view{}) {}
#endif

    // vtb+0
    virtual ~ReadOnlyBinaryStream();

    // vtb+1
    virtual Bedrock::Result<void> read(void *target, size_t num);
};

// size: 104 (1.21.2), 80 (1.21.50/1.21.60)
class BinaryStream : public ReadOnlyBinaryStream {
public:
#if MC_VERSION <= v1_21_2
    std::string mOwnedBuffer;
#endif
    std::string &mBuffer;

    static void *const *__vftable;

#if MC_VERSION == v1_21_2
    BinaryStream(std::string &buffer, bool copyBuffer) :
        ReadOnlyBinaryStream(buffer, copyBuffer), mOwnedBuffer(copyBuffer ? buffer : ""), mBuffer(mOwnedBuffer) {
    }
#elif MC_VERSION == v1_21_50 || v1_21_60
    SDK_API BinaryStream(std::string &buffer, bool copyBuffer);
#    pragma SPHR_LINKER_SYM_ALIAS(                                                                            \
        "??0BinaryStream@@QEAA@AEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z",        \
        "?ctor@BinaryStream@@QEAAPEAV1@AEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z" \
    )

    SDK_API BinaryStream *ctor(std::string &buffer, bool copyBuffer);
#endif

    // vtb+0
    virtual ~BinaryStream();
};