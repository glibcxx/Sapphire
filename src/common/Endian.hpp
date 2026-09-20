#pragma once

#include <array>
#include <bit>
#include <concepts>
#include "Bit.hpp"

namespace sapphire::endian {

    enum class AlignType {
        unaligned,
        aligned,
    };

    template <
        std::integral T,
        std::endian   Endianness,
        AlignType     Alignment,
        std::size_t   Align = Alignment == AlignType::aligned ? alignof(T) : 1>
    class EndianInt {
    private:
        using StorageType = std::array<uint8_t, sizeof(T)>;

        alignas(Align) StorageType mBytes;

        static constexpr StorageType toStorageVal(T hostVal) noexcept {
            return std::bit_cast<StorageType, T>(std::endian::native == Endianness ? hostVal : byteswap(hostVal));
        }

    public:
        constexpr EndianInt() = default;

        constexpr EndianInt(T hostVal) noexcept : mBytes(toStorageVal(hostVal)) {}

        constexpr EndianInt &operator=(T hostVal) noexcept {
            mBytes = toStorageVal(hostVal);
            return *this;
        }

        constexpr operator T() const noexcept {
            T targetVal = std::bit_cast<T>(mBytes);
            return std::endian::native == Endianness ? targetVal : byteswap(targetVal);
        }
    };

    using little8_t = EndianInt<int8_t, std::endian::little, AlignType::unaligned>;
    using little16_t = EndianInt<int16_t, std::endian::little, AlignType::unaligned>;
    using little32_t = EndianInt<int32_t, std::endian::little, AlignType::unaligned>;
    using little64_t = EndianInt<int64_t, std::endian::little, AlignType::unaligned>;

    using ulittle8_t = EndianInt<uint8_t, std::endian::little, AlignType::unaligned>;
    using ulittle16_t = EndianInt<uint16_t, std::endian::little, AlignType::unaligned>;
    using ulittle32_t = EndianInt<uint32_t, std::endian::little, AlignType::unaligned>;
    using ulittle64_t = EndianInt<uint64_t, std::endian::little, AlignType::unaligned>;

    using big8_t = EndianInt<int8_t, std::endian::big, AlignType::unaligned>;
    using big16_t = EndianInt<int16_t, std::endian::big, AlignType::unaligned>;
    using big32_t = EndianInt<int32_t, std::endian::big, AlignType::unaligned>;
    using big64_t = EndianInt<int64_t, std::endian::big, AlignType::unaligned>;

    using ubig8_t = EndianInt<uint8_t, std::endian::big, AlignType::unaligned>;
    using ubig16_t = EndianInt<uint16_t, std::endian::big, AlignType::unaligned>;
    using ubig32_t = EndianInt<uint32_t, std::endian::big, AlignType::unaligned>;
    using ubig64_t = EndianInt<uint64_t, std::endian::big, AlignType::unaligned>;

    template <typename T>
    constexpr bool isEndianInt = false;

    template <std::integral T, std::endian Endianness, AlignType Alignment, std::size_t Align>
    constexpr bool isEndianInt<EndianInt<T, Endianness, Alignment, Align>> = true;

} // namespace sapphire::endian