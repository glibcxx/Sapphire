#include "DragonTextureResourceService.h"

template <>
dragon::ClientTexture mce::CheckedResourceService<dragon::ResolvedTextureResource>::_create<
    mce::TransactionContainer<dragon::res::CreateTextureTransaction, dragon::TextureResourceService>,
    dragon::res::CreateTextureTransaction>(dragon::res::CreateTextureTransaction &payload) {
    using Hook = core::ApiLoader<
        "\xE8\x00\x00\x00\x00\x48\x0F\xBE\x45\x00\x4C\x8D\x3D"_sig,
        &mce::CheckedResourceService<dragon::ResolvedTextureResource>::_create<
            mce::TransactionContainer<dragon::res::CreateTextureTransaction, dragon::TextureResourceService>,
            dragon::res::CreateTextureTransaction>,
        core::deRefCall>;
    return (this->*Hook::origin)(payload);
}

template <>
dragon::ClientTexture mce::CheckedResourceService<dragon::ResolvedTextureResource>::_create<
    mce::TransactionContainer<dragon::res::CreateReadbackTextureTransaction, dragon::TextureResourceService>,
    dragon::res::CreateReadbackTextureTransaction>(dragon::res::CreateReadbackTextureTransaction &payload) {
    using Hook = core::ApiLoader<
        "\xE8\x00\x00\x00\x00\x48\x8B\x9D\x00\x00\x00\x00\x48\x8B\xBD\x00\x00\x00\x00\x48\x89\x5C\x24"_sig,
        &mce::CheckedResourceService<dragon::ResolvedTextureResource>::_create<
            mce::TransactionContainer<dragon::res::CreateReadbackTextureTransaction, dragon::TextureResourceService>,
            dragon::res::CreateReadbackTextureTransaction>,
        core::deRefCall>;
    return (this->*Hook::origin)(payload);
}
