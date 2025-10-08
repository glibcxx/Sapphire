#pragma once

#include <set>
#include "SDK/api/src-deps/Core/File/Path.h"
#include "SDK/api/src-deps/Core/Utility/SemVersion.h"
#include "SDK/api/src-deps/json/Value.h"

namespace cg {

    // size: 32
    struct VariationMap {
    public:
        std::map<std::string, cg::VariationMap> mMaterialJsonVariationMap; // off+0
        Json::Value                             mVariantJson;              // off+16
    };
    static_assert(sizeof(VariationMap) == 32);

    // size: 456 (1.21.2/1.21.50), 368 (1.21.60)
    class RenderMaterialBase {
    public:
        std::map<std::string, std::shared_ptr<cg::RenderMaterialBase>> mMaterialVariations; // off+8

        cg::VariationMap mVariationMap; // off+24

        std::string mFullMaterialName;   // off+56
        std::string mVariantName;        // off+88
        std::string mMaterialIdentifier; // off+120

        std::set<std::string> mDefines; // off+152

        uint16_t mStateMask;                             // off+168
                                                         // gap 6 byte
        std::string                   mShaderHeader;     // off+176
        Core::PathBuffer<std::string> mVertexShader;     // off+208
        Core::PathBuffer<std::string> mFragmentShader;   // off+240
        Core::PathBuffer<std::string> mGeometryShader;   // off+272
        Core::PathBuffer<std::string> mVRGeometryShader; // off+304
        SemVersion                    mVersion;          // off+336
        bool                          mUnkByte;

        virtual ~RenderMaterialBase();
    };

} // namespace cg