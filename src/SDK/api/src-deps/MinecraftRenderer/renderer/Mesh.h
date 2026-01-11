#pragma once

#include "SDK/api/src-deps/MinecraftRenderer/Resources/IndexBufferContainer.h"
#include "MeshData.h"
#include "VertexLayout.h"

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
            mVertexBuffer; // off+408
        mce::ClientResourcePointer<
            std::variant<
                std::monostate,
                mce::Buffer,
                mce::ClientResourcePointer<dragon::ResolvedVertexBufferResource>,
                mce::ClientResourcePointer<dragon::ResolvedIndexBufferResource>>>
                                mAttributeBuffer; // off+432
        std::optional<uint32_t> mVertexCount;     // off+456
        VertexLayout            mVertexFormat;    // off+464
        std::vector<byte>       mRawData;         // off+536

        SDK_API Mesh(Mesh &&other);

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xC8\xEB\x00\x33\xC9\x48\x89\x4C\x24")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x33\xF6\xEB\x00\x33\xF6\x8B\xC6\x48\x89\x44\x24")
        SPHR_CTOR_ALIAS SDK_API Mesh *ctor(mce::Mesh &&c);

        SDK_API ~Mesh() noexcept;

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x48\x83\xC3\x00\x48\x3B\xDF\x75\x00\x48\x8B\x7C\x24\x00\x48\x8B\x5C\x24\x00\xC6\x86")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x41\xFF\xC4\x45\x3B\xE5\x0F\x8C")
        SPHR_DTOR_ALIAS SDK_API void dtor() noexcept;

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\x48\xB8")
        SDK_API bool isValid() const;

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x84\xC0\x74\x00\xF3\x0F\x10\x45\x00\xF3\x41\x0F\x5C\xC2")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x84\xC0\x74\x00\x41\x0F\x2E\x77")
        SDK_API bool areBuffersValid() const;
    };
#if MC_VERSION == v1_21_2
    static_assert(sizeof(Mesh) == 536);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    static_assert(sizeof(Mesh) == 560);
#endif

} // namespace mce