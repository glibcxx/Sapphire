#include "GraphicsPlatform.h"

namespace dragon::platform {

    GraphicsPlatform *GraphicsPlatform::ctor(GraphicsPlatform &&that) {
        using Hook = sapphire::ApiLoader<
            "\x0F\xB6\x02\x0F\x57\xC0\x88\x01\x0F\xB6\x42\x00\x88\x41\x00\x33\xC0"_sig, // 1.21.50
            (GraphicsPlatform * (GraphicsPlatform::*)(GraphicsPlatform &&)) & GraphicsPlatform::ctor,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(std::move(that));
    }

    GraphicsPlatform *GraphicsPlatform::ctor(const GraphicsPlatform &that) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\xC6\x84\x24\x00\x00\x00\x00\x00\x48\x8D\x4F"_sig, // 1.21.50
            (GraphicsPlatform * (GraphicsPlatform::*)(const GraphicsPlatform &)) & GraphicsPlatform::ctor,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(std::move(that));
    }

} // namespace dragon::platform
