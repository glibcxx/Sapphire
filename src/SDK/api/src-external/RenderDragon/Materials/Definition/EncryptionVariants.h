#pragma once

#include <string>
#include <variant>

namespace dragon::materials::definition {

    struct EncryptionVariants {
        struct SimplePassphrase {
            std::string                         key;  // off+0
            std::variant<std::string /*, ???*/> mUnk; // off+32, size: 40
        };
    };

} // namespace dragon::materials::definition