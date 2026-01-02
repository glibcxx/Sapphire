#pragma once

#include "RenderAPI.h"
namespace dragon::platform {

    // size: 208 (1.21.50)
    struct Statistics {
        int64_t  mCpuTimeFrame;            // off+0
        int64_t  mCpuTimeBegin;            // off+8
        int64_t  mCpuTimeEnd;              // off+16
        int64_t  mCpuTimerFreq;            // off+24
        int64_t  mGpuTimeBegin;            // off+32
        int64_t  mGpuTimeEnd;              // off+40
        int64_t  mGpuTimerFreq;            // off+48
        int64_t  mWaitRender;              // off+56
        int64_t  mWaitSubmit;              // off+64
        uint32_t mNumDraw;                 // off+72
        uint32_t mNumCompute;              // off+76
        uint32_t mMaxGpuLatency;           // off+80
        uint16_t mNumDynamicIndexBuffers;  // off+84
        uint16_t mNumDynamicVertexBuffers; // off+86
        uint16_t mNumFrameBuffers;         // off+88
        uint16_t mNumIndexBuffers;         // off+90
        uint16_t mNumOcclusionQueries;     // off+92
        uint16_t mNumPrograms;             // off+94
        uint16_t mNumShaders;              // off+96
        uint16_t mNumTextures;             // off+98
        uint16_t mNumUniforms;             // off+100
        uint16_t mNumVertexBuffers;        // off+102
        uint16_t mNumVertexDecls;          // off+104
        int64_t  mTextureMemoryUsed;       // off+112
        int64_t  mRtMemoryUsed;            // off+120
        int32_t  mTransientVbUsed;         // off+128
        int32_t  mTransientIbUsed;         // off+132
        uint64_t mGpuMemoryMax;            // off+136
        uint64_t mGpuMemoryUsed;           // off+144

        struct static_vector_unsigned_char_16_ {
            unsigned char m_data[16];
            size_t        m_msize;
        };

        static_vector_unsigned_char_16_ mSupportedMSAALevels; // off+152
        RenderAPI                       mRenderApi;           // off+176
        __int8                          mUnk177;              // off+177
        std::vector<std::string>        mUnk184;              // off+184
    };

} // namespace dragon::platform