#include "VertexDeclManager.h"

namespace dragon::mesh {

    VertexDeclManager::Impl *VertexDeclManager::Impl::ctor() {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xC7\x49\x89\x04\x24"_sig, // 1.21.50
            &Impl::ctor,
            core::deRefCall>;
        return (this->*Hook::origin)();
    }

} // namespace dragon::mesh