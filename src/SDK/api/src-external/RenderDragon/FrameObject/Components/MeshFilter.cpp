#include "MeshFilter.h"

void DFC::MeshFilter::dtor() noexcept {
    using Hook = sapphire::ApiLoader<
        "\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x41\x39\x5E"_sig,
        &MeshFilter::dtor,
        sapphire::deRefCall>;
    (this->*Hook::origin)();
}