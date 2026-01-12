#pragma once

#include <bit>
#include <cstdint>
#include <type_traits>

namespace sapphire {

    template <typename T>
    constexpr T &dAccess(void *obj, uintptr_t offset) {
        return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(obj) + offset);
    }

    template <typename T>
        requires(std::is_reference_v<T>)
    constexpr T &&dAccess(void *obj, uintptr_t offset) {
        using T_ = std::remove_reference_t<T>;
        return **reinterpret_cast<T_ **>(reinterpret_cast<uintptr_t>(obj) + offset);
    }

    template <typename T>
    constexpr const T &dAccess(const void *obj, uintptr_t offset) {
        return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(obj) + offset);
    }

    template <typename T>
        requires(std::is_reference_v<T>)
    constexpr const T &&dAccess(const void *obj, uintptr_t offset) {
        using T_ = std::remove_reference_t<T>;
        return **reinterpret_cast<T_ **>(reinterpret_cast<uintptr_t>(obj) + offset);
    }

    template <typename T = void **>
    constexpr T getVftable(void *obj) {
        return std::bit_cast<T>(*reinterpret_cast<void ***>(obj));
    }

    template <typename T = void **>
    constexpr T getVftable(const void *obj) {
        return std::bit_cast<T>(*reinterpret_cast<void ***>(const_cast<void *>(obj)));
    }

    template <typename Fn>
        requires(std::is_member_function_pointer_v<Fn> && sizeof(Fn) == 2 * sizeof(uintptr_t))
    constexpr uintptr_t toFuncAddr(Fn addr) {
        struct member_func_ptr {
            uintptr_t addr;
            uintptr_t offset;
        };
        return std::bit_cast<member_func_ptr>(addr).addr;
    }

    template <typename Fn>
        requires(std::is_member_function_pointer_v<Fn> && sizeof(Fn) == sizeof(uintptr_t) || std::is_function_v<std::remove_pointer_t<Fn>>)
    constexpr uintptr_t toFuncAddr(Fn addr) {
        return std::bit_cast<uintptr_t>(addr);
    }

    constexpr uintptr_t toFuncAddr(uintptr_t addr) {
        return addr;
    }

    template <typename Fn>
        requires(std::is_member_function_pointer_v<Fn> && sizeof(Fn) == 2 * sizeof(uintptr_t))
    constexpr Fn toMemberFunc(uintptr_t addr) {
        struct member_func_ptr {
            uintptr_t addr;
            uintptr_t offset;
        };
        return std::bit_cast<Fn>(member_func_ptr{addr, 0});
    }

    template <typename Fn>
        requires(std::is_member_function_pointer_v<Fn> && sizeof(Fn) == sizeof(uintptr_t) || std::is_function_v<std::remove_pointer_t<Fn>>)
    constexpr Fn toMemberFunc(uintptr_t addr) {
        return std::bit_cast<Fn>(addr);
    }

    template <typename Ret = void, typename... Args>
    Ret vCall(void *obj, size_t fnIdx, Args &&...args) {
        return reinterpret_cast<Ret (*)(void *, Args &&...)>(
            getVftable(obj)[fnIdx]
        )(obj, std::forward<Args>(args)...);
    }

    template <typename Ret = void, typename... Args>
    Ret vCall(const void *obj, size_t fnIdx, Args &&...args) {
        return reinterpret_cast<Ret (*)(const void *, Args &&...)>(
            getVftable(obj)[fnIdx]
        )(obj, std::forward<Args>(args)...);
    }

    template <typename Ret = void, typename... Args>
    Ret vCallOffset(void *obj, intptr_t offset, size_t fnIdx, Args &&...args) {
        return reinterpret_cast<Ret (*)(void *, Args &&...)>(
            getVftable(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(obj) + offset))[fnIdx]
        )(obj, std::forward<Args>(args)...);
    }

    inline uintptr_t ripRel(uintptr_t instrcutionAddr, uint32_t offset, uint32_t insLen) {
        int32_t   displacementAddr = *reinterpret_cast<int32_t *>(instrcutionAddr + offset);
        uintptr_t nextInstructionAddr = instrcutionAddr + insLen;
        uintptr_t targetAddr = nextInstructionAddr + displacementAddr;
        return targetAddr;
    }

    enum class InstType {
        LEA = 0,  // e.g. 4C 8D 0D 11 22 33 44  lea   r9, sub_11451419
        CALL = 1, // e.g. E8       11 22 33 44  call  sub_11451419
        MOV = 2,  // e.g. 48 8B 05 55 66 77 88  mov   rax, cs:qword_1919810
    };

    inline uintptr_t ripRel(uintptr_t instrcutionAddr, InstType opType) {
        int instructionLength = 0;
        int displacementOffset = 0;
        switch (opType) {
        case InstType::LEA:
        case InstType::MOV:
            instructionLength = 7;
            displacementOffset = 3;
            break;
        case InstType::CALL:
            instructionLength = 5;
            displacementOffset = 1;
            break;
        default:
            return 0;
        }
        return ripRel(instrcutionAddr, displacementOffset, instructionLength);
    }

    template <typename T>
    constexpr T ripRel(T instrcutionAddr, InstType opType) {
        return std::bit_cast<T>(ripRel(std::bit_cast<uintptr_t>(instrcutionAddr), opType));
    }

    template <typename TField, size_t Offset>
    struct Field {};

    template <typename TField, size_t Offset>
    constexpr Field<TField, Offset> field{};

    template <typename T, typename TField, size_t Offset>
    decltype(auto) operator->*(T *obj, Field<TField, Offset>) {
        return dAccess<TField>(obj, Offset);
    }

    template <typename T, typename TField, size_t Offset>
    decltype(auto) operator->*(T &obj, Field<TField, Offset>) {
        return dAccess<TField>(&obj, Offset);
    }

} // namespace sapphire