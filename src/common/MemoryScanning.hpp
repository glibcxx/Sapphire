#pragma once

#include <immintrin.h>
#include <array>
#include "Memory.hpp"
#include "../macros/Macros.h"

namespace sapphire {

    SPHR_FORCE_INLINE inline uintptr_t scanSignatureBruteForce(uintptr_t start, size_t size, const char *sig, size_t sigLength) {
        if (!sigLength || !size) return 0;
        const char *data = reinterpret_cast<const char *>(start);
        for (uintptr_t i = 0; i <= size - sigLength; ++i) {
            bool match = true;
            for (uintptr_t j = 0; j < sigLength; ++j) {
                if (sig[j] != 0x00 && data[i + j] != sig[j]) // 0x00表示通配符
                {
                    match = false;
                    break;
                }
            }
            if (match) return start + i;
        }
        return 0;
    }

    SPHR_FORCE_INLINE inline uintptr_t scanSignatureBMH(uintptr_t start, size_t size, const char *sig, size_t sigLength) {
        SPHR_ASSUME(sigLength < 256)
        if (!sigLength || !size) return 0;
        uint8_t init = sigLength;
        for (size_t i = sigLength - 1; i > 0; --i) {
            if (sig[i] == 0x00) {
                init = sigLength - 1 - i;
                break;
            }
        }
        std::array<uint8_t, 256> skipTable;
        skipTable.fill(init);
        for (size_t i = 0; i < sigLength - 1; i++) {
            if (sig[i] != 0x00)
                skipTable[static_cast<uint8_t>(sig[i])] = std::min<uint8_t>(sigLength - 1 - i, init);
        }
        const char *data = reinterpret_cast<const char *>(start);
        for (uintptr_t sigBack = sigLength - 1; sigBack < size;) {
            bool match = true;
            for (uintptr_t iterSig = 0; iterSig < sigLength; ++iterSig) {
                char sigChar = sig[sigLength - 1 - iterSig];
                char dataChar = data[sigBack - iterSig];
                if (sigChar == 0x00 || sigChar == data[sigBack - iterSig])
                    continue;
                sigBack += skipTable[static_cast<uint8_t>(dataChar)];
                match = false;
                break;
            }
            if (match) return start + sigBack - sigLength + 1;
        }

        return 0;
    }

    SPHR_FORCE_INLINE inline uintptr_t scanSignature(uintptr_t start, size_t size, const char *sig, size_t sigLength) {
        const size_t THRESHOLD = 32;
        if (sigLength < THRESHOLD) {
            return scanSignatureBruteForce(start, size, sig, sigLength);
        } else {
            return scanSignatureBMH(start, size, sig, sigLength);
        }
    }

} // namespace sapphire::memory