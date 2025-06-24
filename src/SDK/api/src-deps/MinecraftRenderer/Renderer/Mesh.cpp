#include "Mesh.h"

namespace mce {

    bool mce::Mesh::isValid() const { // \x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\x48\xB8 1.21.50
        return true;
    }

    Mesh *Mesh::ctor(mce::Mesh &&c) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x48\x8B\x87"_sig, // 1.21.50
            &Mesh::ctor,
            core::deRefCall>;
        return (this->*Hook::origin)(std::move(c));
    }

} // namespace mce