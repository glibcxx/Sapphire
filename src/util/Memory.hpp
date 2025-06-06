#pragma once

#include <bit>
#include <cstdint>
#include <type_traits>

namespace memory {
    template <typename T>
    T &getField(void *obj, uintptr_t offset) {
        return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(obj) + offset);
    }

    template <typename T>
        requires(std::is_reference_v<T>)
    T &&getField(void *obj, uintptr_t offset) {
        using T_ = std::remove_reference_t<T>;
        return **reinterpret_cast<T_ **>(reinterpret_cast<uintptr_t>(obj) + offset);
    }

    template <typename T>
    const T &getField(const void *obj, uintptr_t offset) {
        return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(obj) + offset);
    }

    template <typename T>
        requires(std::is_reference_v<T>)
    const T &&getField(const void *obj, uintptr_t offset) {
        using T_ = std::remove_reference_t<T>;
        return **reinterpret_cast<T_ **>(reinterpret_cast<uintptr_t>(obj) + offset);
    }

    template <typename T = void **>
    T getVftable(void *obj) {
        return std::bit_cast<T>(*reinterpret_cast<void ***>(obj));
    }

    template <typename T = void **>
    T getVftable(const void *obj) {
        return std::bit_cast<T>(*reinterpret_cast<void ***>(const_cast<void *>(obj)));
    }

    template <typename Fn>
        requires(std::is_member_function_pointer_v<Fn> && sizeof(Fn) == 2 * sizeof(uintptr_t))
    uintptr_t toRawFunc(Fn addr) {
        struct member_func_ptr {
            uintptr_t addr;
            uintptr_t offset;
        };
        return std::bit_cast<member_func_ptr>(addr).addr;
    }

    template <typename Fn>
        requires(std::is_member_function_pointer_v<Fn> && sizeof(Fn) == sizeof(uintptr_t) || std::is_function_v<std::remove_pointer_t<Fn>>)
    uintptr_t toRawFunc(Fn addr) {
        return std::bit_cast<uintptr_t>(addr);
    }

    template <typename Fn>
        requires(std::is_member_function_pointer_v<Fn> && sizeof(Fn) == 2 * sizeof(uintptr_t))
    Fn toMemberFunc(uintptr_t addr) {
        struct member_func_ptr {
            uintptr_t addr;
            uintptr_t offset;
        };
        return std::bit_cast<Fn>(member_func_ptr{addr, 0});
    }

    template <typename Fn>
        requires(std::is_member_function_pointer_v<Fn> && sizeof(Fn) == sizeof(uintptr_t) || std::is_function_v<std::remove_pointer_t<Fn>>)
    Fn toMemberFunc(uintptr_t addr) {
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

    enum class AsmOperation {
        LEA = 0,  // e.g. 4C 8D 0D 11 22 33 44  lea   r9, sub_11451419
        CALL = 1, // e.g. E8       11 22 33 44  call  sub_11451419
        MOV = 2,  // e.g. 48 8B 05 55 66 77 88  mov   rax, cs:qword_1919810
    };

    inline uintptr_t deRef(uintptr_t instrcutionAddr, AsmOperation opType) {
        int instruction_length = 0;
        int displacement_offset = 0;
        switch (opType) {
        case AsmOperation::LEA:
        case AsmOperation::MOV:
            instruction_length = 7;
            displacement_offset = 3;
            break;
        case AsmOperation::CALL:
            instruction_length = 5;
            displacement_offset = 1;
            break;
        default:
            return 0;
        }
        uint8_t  *instruction_ptr = reinterpret_cast<uint8_t *>(instrcutionAddr);
        int32_t   displacement = *reinterpret_cast<int32_t *>(instruction_ptr + displacement_offset);
        uintptr_t next_instruction_addr = instrcutionAddr + instruction_length;
        uintptr_t target_addr = next_instruction_addr + displacement;
        return target_addr;
    }

    template <typename T>
    T deRef(T instrcutionAddr, AsmOperation opType) {
        return std::bit_cast<T>(deRef(std::bit_cast<uintptr_t>(instrcutionAddr), opType));
    }

} // namespace memory