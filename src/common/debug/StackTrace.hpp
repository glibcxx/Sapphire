#pragma once

#include <format>
#include <vector>
#include "Symbolizer.hpp"
#include "macros/Macros.h"

namespace sapphire::debug {

    class StackFrame {
    public:
        constexpr StackFrame() noexcept = default;
        explicit constexpr StackFrame(uintptr_t address) noexcept : mAddress(address) {}

        [[nodiscard]] constexpr uintptr_t address() const noexcept {
            return mAddress;
        }

        [[nodiscard]] constexpr explicit operator bool() const noexcept {
            return mAddress != 0;
        }

        // 符号反查（懒解析：首次访问时解析并持久化缓存到成员中）
        [[nodiscard]] const SymbolInfo &symbolInfo() const noexcept {
            if (!mResolved && mAddress != 0) {
                Symbolizer::instance().resolve(mAddress, mSymbolInfo);
                mResolved = true;
            }
            return mSymbolInfo;
        }

        [[nodiscard]] std::string_view moduleName() const noexcept {
            const auto &info = symbolInfo();
            return info.mHasModule ? std::string_view(info.mModuleName) : std::string_view{};
        }

        [[nodiscard]] std::string_view functionName() const noexcept {
            const auto &info = symbolInfo();
            return info.mHasSymbol ? std::string_view(info.mFunctionName) : std::string_view{};
        }

        [[nodiscard]] std::string_view sourceFile() const noexcept {
            const auto &info = symbolInfo();
            return info.mHasLine ? std::string_view(info.mFileName) : std::string_view{};
        }

        [[nodiscard]] uint32_t sourceLine() const noexcept {
            return symbolInfo().mLineNumber;
        }

        [[nodiscard]] std::string toString() const noexcept {
            return Symbolizer::instance().format(symbolInfo());
        }

        constexpr auto operator<=>(const StackFrame &rhs) const noexcept {
            return mAddress <=> rhs.mAddress;
        }

        constexpr bool operator==(const StackFrame &rhs) const noexcept {
            return mAddress == rhs.mAddress;
        }

    private:
        uintptr_t          mAddress = 0;
        mutable bool       mResolved = false;
        mutable SymbolInfo mSymbolInfo{};
    };

    class StackTrace {
    public:
        inline static constexpr size_t DEFAULT_MAX_DEPTH = 64;

        StackTrace() = default;
        explicit StackTrace(std::vector<StackFrame> frames) noexcept : mFrames(std::move(frames)) {}

        /**
         * @brief 逐帧懒遍历当前线程栈 (零堆分配，适合轻量采集)
         * @param visitor 回调签名：bool(const StackFrame& frame)，返回 false 提前终止
         */
        template <typename FrameVisitor>
        SPHR_FORCE_INLINE static void walkCurrent(FrameVisitor &&visitor, size_t skipCount = 0, size_t maxDepth = DEFAULT_MAX_DEPTH) noexcept {
            constexpr size_t LOCAL_BUFFER_CAPACITY = 128;
            void            *buffer[LOCAL_BUFFER_CAPACITY] = {};

            const DWORD  countToCapture = static_cast<DWORD>(std::min(maxDepth, LOCAL_BUFFER_CAPACITY));
            const USHORT captured = CaptureStackBackTrace(
                static_cast<DWORD>(skipCount),
                countToCapture,
                buffer,
                nullptr
            );

            for (USHORT i = 0; i < captured; ++i) {
                StackFrame frame(reinterpret_cast<uintptr_t>(buffer[i]));
                if (!visitor(frame)) {
                    break;
                }
            }
        }

        /**
         * @brief 逐帧懒遍历指定上下文栈 (用于崩溃线程分析)
         * @param context 寄存器上下文 (EXCEPTION_POINTERS->ContextRecord)
         * @param hThread 目标线程句柄
         */
        template <typename FrameVisitor>
        static void walkContext(const CONTEXT &context, HANDLE hThread, FrameVisitor &&visitor, size_t maxDepth = DEFAULT_MAX_DEPTH) noexcept {
            HANDLE  hProcess = GetCurrentProcess();
            CONTEXT ctxCopy = context; // StackWalk64 会修改 context 寄存器，做本地副本

            STACKFRAME64 frame{};
            DWORD        machineType = 0;

#if defined(_M_X64) || defined(__x86_64__)
            machineType = IMAGE_FILE_MACHINE_AMD64;
            frame.AddrPC.Offset = ctxCopy.Rip;
            frame.AddrPC.Mode = AddrModeFlat;
            frame.AddrFrame.Offset = ctxCopy.Rbp;
            frame.AddrFrame.Mode = AddrModeFlat;
            frame.AddrStack.Offset = ctxCopy.Rsp;
            frame.AddrStack.Mode = AddrModeFlat;
#elif defined(_M_IX86) || defined(__i386__)
            machineType = IMAGE_FILE_MACHINE_I386;
            frame.AddrPC.Offset = ctxCopy.Eip;
            frame.AddrPC.Mode = AddrModeFlat;
            frame.AddrFrame.Offset = ctxCopy.Ebp;
            frame.AddrFrame.Mode = AddrModeFlat;
            frame.AddrStack.Offset = ctxCopy.Esp;
            frame.AddrStack.Mode = AddrModeFlat;
#elif defined(_M_ARM64)
            machineType = IMAGE_FILE_MACHINE_ARM64;
            frame.AddrPC.Offset = ctxCopy.Pc;
            frame.AddrPC.Mode = AddrModeFlat;
            frame.AddrFrame.Offset = ctxCopy.Fp;
            frame.AddrFrame.Mode = AddrModeFlat;
            frame.AddrStack.Offset = ctxCopy.Sp;
            frame.AddrStack.Mode = AddrModeFlat;
#else
            return;
#endif

            size_t depth = 0;
            while (depth < maxDepth) {
                const BOOL success = StackWalk64(
                    machineType,
                    hProcess,
                    hThread,
                    &frame,
                    &ctxCopy,
                    nullptr,
                    SymFunctionTableAccess64,
                    SymGetModuleBase64,
                    nullptr
                );

                if (!success || frame.AddrPC.Offset == 0) {
                    break;
                }

                StackFrame sf(static_cast<uintptr_t>(frame.AddrPC.Offset));
                if (!visitor(sf)) {
                    break;
                }
                ++depth;
            }
        }

        [[nodiscard]] SPHR_NOINLINE static StackTrace current(size_t skipCount = 0, size_t maxDepth = DEFAULT_MAX_DEPTH) {
            StackTrace trace;
            trace.mFrames.reserve(std::min(maxDepth, size_t(64)));

            walkCurrent(
                [&trace](const StackFrame &frame) {
                    trace.mFrames.push_back(frame);
                    return true;
                },
                skipCount + 1, // +1 过滤掉 current 自身
                maxDepth
            );

            return trace;
        }

        [[nodiscard]] static StackTrace fromContext(const CONTEXT &context, HANDLE hThread = GetCurrentThread(), size_t maxDepth = DEFAULT_MAX_DEPTH) {
            StackTrace trace;
            trace.mFrames.reserve(std::min(maxDepth, size_t(64)));

            walkContext(
                context,
                hThread,
                [&trace](const StackFrame &frame) {
                    trace.mFrames.push_back(frame);
                    return true;
                },
                maxDepth
            );

            return trace;
        }

        [[nodiscard]] bool              empty() const noexcept { return mFrames.empty(); }
        [[nodiscard]] size_t            size() const noexcept { return mFrames.size(); }
        [[nodiscard]] const StackFrame &operator[](size_t index) const { return mFrames[index]; }
        [[nodiscard]] const StackFrame &at(size_t index) const { return mFrames.at(index); }

        [[nodiscard]] auto begin() const noexcept { return mFrames.begin(); }
        [[nodiscard]] auto end() const noexcept { return mFrames.end(); }
        [[nodiscard]] auto cbegin() const noexcept { return mFrames.cbegin(); }
        [[nodiscard]] auto cend() const noexcept { return mFrames.cend(); }

        [[nodiscard]] std::span<const StackFrame> asSpan() const noexcept { return mFrames; }

        [[nodiscard]] std::string toString(size_t indent = 2) const {
            std::string result;
            toString(std::back_inserter(result), indent);
            return result;
        }

        template <std::output_iterator<const char &> OutIter>
        OutIter toString(OutIter out, size_t indent = 2) const {
            std::string indentStr(indent, ' ');
            for (size_t i = 0; i < mFrames.size(); ++i) {
                std::format_to(out, "{}[{:2}] {}\n", indentStr, i, mFrames[i].toString());
            }
            return out;
        }

        [[nodiscard]] SPHR_FORCE_INLINE static std::string currentToString(size_t skipCount = 0, size_t maxDepth = DEFAULT_MAX_DEPTH) {
            return current(skipCount, maxDepth).toString();
        }

    private:
        std::vector<StackFrame> mFrames;
    };

} // namespace sapphire::debug