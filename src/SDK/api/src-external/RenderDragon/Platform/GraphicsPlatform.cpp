#include "GraphicsPlatform.h"

namespace dragon::platform {

    GraphicsPlatform *GraphicsPlatform::ctor(GraphicsPlatform &&that) {
        using Fn = GraphicsPlatform *(GraphicsPlatform::*)(GraphicsPlatform &&);
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\x0F\xB6\x02\x0F\x57\xC0\x88\x01\x0F\xB6\x42\x00\x88\x41\x00\x33\xC0"_sig,
            (Fn)&GraphicsPlatform::ctor>;
        return (this->*Hook::origin)(std::move(that));
    }

    GraphicsPlatform *GraphicsPlatform::ctor(const GraphicsPlatform &that) {
        using Fn = GraphicsPlatform *(GraphicsPlatform::*)(const GraphicsPlatform &);
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\xBB\x00\x00\x00\x00\xEB\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x90\xBB\x00\x00\x00\x00\x89\x5C\x24\x00\x48\x8B\xD0\x48\x8B\xCE"_sig,
            (Fn)&GraphicsPlatform::ctor>;
        return (this->*Hook::origin)(std::move(that));
    }

} // namespace dragon::platform
