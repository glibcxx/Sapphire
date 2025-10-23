#include "BinaryStream.h"
#include "SDK/core/ApiManager.h"

void *const *BinaryStream::__vftable = sapphire::loadVftable<
#if MC_VERSION == v1_21_2
    "\x48\x8D\x05\x00\x00\x00\x00\x48\x8B\xD9\x48\x89\x01\x48\x83\xC1\x00\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x4B"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    "\x48\x8D\x05\x00\x00\x00\x00\x48\xC7\x43\x00\x00\x00\x00\x00\x48\x89\x03\x48\x8B\xC3\x0F\x11\x43"_sig,
#endif
    &BinaryStream::__vftable>();

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
BinaryStream *BinaryStream::ctor(std::string &buffer, bool copyBuffer) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x89\x4C\x24\x00\x48\x8B\xFA"_sig,
        &BinaryStream::ctor>;
    return (this->*Hook::origin)(buffer, copyBuffer);
}
#endif
