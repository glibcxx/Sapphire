#pragma once

#include <cstring>
#include <span>
#include <system_error>
#include <vector>
#include "Endian.hpp"
#include "Expected.hpp"

namespace sapphire {

    class ByteWriter {
        std::vector<uint8_t> mBuffer;

    public:
        constexpr explicit ByteWriter(int size = 0) { mBuffer.reserve(size); }

        template <typename T>
            requires std::integral<T> || std::floating_point<T> || endian::isEndianInt<T>
        constexpr void write(const T &val) {
            const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&val);
            mBuffer.insert(mBuffer.end(), ptr, ptr + sizeof(T));
        }

        constexpr void writeBytes(std::span<const uint8_t> data) {
            mBuffer.insert(mBuffer.end(), data.begin(), data.end());
        }

        constexpr std::vector<uint8_t> extract() { return std::move(mBuffer); }
    };

    class ByteReader {
        std::span<const uint8_t> mData;
        size_t                   mOffset = 0;

    public:
        constexpr ByteReader(std::span<const uint8_t> data) noexcept : mData(data) {}

        template <typename T>
        Expected<T, std::error_code> read() noexcept {
            static_assert(false, "unsupported type!");
        }

        template <typename T>
            requires std::integral<T> || std::floating_point<T> || endian::isEndianInt<T>
        Expected<T, std::error_code> read() noexcept {
            if (mOffset + sizeof(T) > mData.size())
                return {unexpected, std::make_error_code(std::errc::result_out_of_range)};
            T val;
            std::memcpy(&val, mData.data() + mOffset, sizeof(T));
            mOffset += sizeof(T);
            return val;
        }

        template <std::same_as<std::string_view> T>
        constexpr Expected<std::string_view, std::error_code> read() noexcept {
            size_t len = 0;
            while (mOffset + len < mData.size() && mData[mOffset + len] != '\0') ++len;
            if (mOffset + len >= mData.size())
                return {unexpected, std::make_error_code(std::errc::result_out_of_range)};
            std::string_view str{reinterpret_cast<const char *>(mData.data() + mOffset), len};
            mOffset += len + 1;
            return str;
        }

        template <std::same_as<std::span<const uint8_t>> T>
        constexpr Expected<std::span<const uint8_t>, std::error_code> read(size_t len) noexcept {
            if (mOffset + len > mData.size()) return {unexpected, std::make_error_code(std::errc::result_out_of_range)};
            auto view = mData.subspan(mOffset, len);
            mOffset += len;
            return view;
        }

        constexpr Expected<std::span<const uint8_t>, std::error_code> readSpan(size_t len) noexcept {
            return read<std::span<const uint8_t>>(len);
        }

        constexpr Expected<uint8_t, std::error_code> peek() const noexcept {
            if (mOffset >= mData.size()) return {unexpected, std::make_error_code(std::errc::result_out_of_range)};
            return mData[mOffset];
        }

        constexpr void   skip(size_t bytes) noexcept { mOffset += bytes; }
        constexpr size_t offset() const noexcept { return mOffset; }
        constexpr void   setOffset(size_t pos) noexcept { mOffset = pos; }
    };

} // namespace sapphire