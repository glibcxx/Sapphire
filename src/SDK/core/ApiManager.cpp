#include "ApiManager.h"

#include "util/MemoryScanning.hpp"

namespace sapphire {

    uintptr_t ApiManager::_scanApi(const char *sig, size_t sigLength) {
        return memory::scanSignature(
            this->mMainModuleInfo.mTextSectionStart,
            this->mMainModuleInfo.mTextSectionSize,
            sig,
            sigLength
        );
    }

    ApiManager &ApiManager::getInstance() {
        static ApiManager instance;
        return instance;
    }


} // namespace sapphire