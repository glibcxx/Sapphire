#include "DragonTextureResourceService.h"

template <>
dragon::ClientTexture mce::CheckedResourceService<dragon::ResolvedTextureResource>::_create<
    mce::TransactionContainer<dragon::res::CreateTextureTransaction, dragon::TextureResourceService>,
    dragon::res::CreateTextureTransaction>(dragon::res::CreateTextureTransaction &payload) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x89\x7C\x24\x00\x48\x8B\x45"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x0F\xBE\x45\x00\x4C\x8D\x3D"_sig,
#endif
        &mce::CheckedResourceService<dragon::ResolvedTextureResource>::_create<
            mce::TransactionContainer<dragon::res::CreateTextureTransaction, dragon::TextureResourceService>,
            dragon::res::CreateTextureTransaction>>;
    return (this->*Bind::origin)(payload);
}

template <>
dragon::ClientTexture mce::CheckedResourceService<dragon::ResolvedTextureResource>::_create<
    mce::TransactionContainer<dragon::res::CreateReadbackTextureTransaction, dragon::TextureResourceService>,
    dragon::res::CreateReadbackTextureTransaction>(dragon::res::CreateReadbackTextureTransaction &payload) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x85\x00\x00\x00\x00\x48\x8B\x8D\x00\x00\x00\x00\x48\x89\x45"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x9D\x00\x00\x00\x00\x48\x8B\xBD\x00\x00\x00\x00\x48\x89\x5C\x24"_sig,
#endif
        &mce::CheckedResourceService<dragon::ResolvedTextureResource>::_create<
            mce::TransactionContainer<dragon::res::CreateReadbackTextureTransaction, dragon::TextureResourceService>,
            dragon::res::CreateReadbackTextureTransaction>>;
    return (this->*Bind::origin)(payload);
}
