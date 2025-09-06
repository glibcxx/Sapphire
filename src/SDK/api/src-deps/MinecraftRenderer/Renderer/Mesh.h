#pragma once

#include "SDK/api/src-deps/MinecraftRenderer/Resources/IndexBufferContainer.h"
#include "MeshData.h"

namespace mce {

    class BufferResourceService;

    // size: 536 (1.21.2), 560 (1.21.50/1.21.60)
    class Mesh : public IndexBufferContainer {
    public:
        std::variant<std::monostate, uint64_t, glm::tvec3<int>> mCacheKey; // off+40

        bool                                 mTemporary;             // off+64
        PrimitiveMode                        mPrimitiveMode;         // off+65
        std::string                          mDebugName;             // off+72
        std::weak_ptr<BufferResourceService> mBufferResourceService; // off+104
        MeshData                             mMeshData;              // off+120

        //=============================================== // (1.21.50/1.21.60), (1.21.2 = 1.21.50 - 24)
        mce::ClientResourcePointer<
            std::variant<
                std::monostate,
                mce::Buffer,
                mce::ClientResourcePointer<dragon::ResolvedVertexBufferResource>,
                mce::ClientResourcePointer<dragon::ResolvedIndexBufferResource>>>
                                        mVertexBuffer;    // off+408
        mce::ClientResourcePointer<int> mUnkBuffer;       // off+432
        std::optional<uint32_t>         mVertexCount;     // off+456
        uint64_t                        mVertexFormat[9]; // off+464
        std::vector<byte>               mRawData;         // off+536

        SDK_API bool isValid() const;

        SDK_API Mesh(Mesh &&other);
#pragma SPHR_LINKER_SYM_ALIAS("??0Mesh@mce@@QEAA@$$QEAV01@@Z", "?ctor@Mesh@mce@@QEAAPEAV12@$$QEAV12@@Z")

        SDK_API Mesh *ctor(mce::Mesh &&c);
    };
#if MC_VERSION == v1_21_2
    static_assert(sizeof(Mesh) == 536);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    static_assert(sizeof(Mesh) == 560);
#endif

} // namespace mce