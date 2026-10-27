#pragma once

namespace Bedrock::Threading {

    // size: 4
    class OSThreadPriority {
    public:
        static const Bedrock::Threading::OSThreadPriority High;
        static const Bedrock::Threading::OSThreadPriority Elevated;
        static const Bedrock::Threading::OSThreadPriority Normal;
        static const Bedrock::Threading::OSThreadPriority Low;

        constexpr OSThreadPriority(int p) : mPriority(p) {}
        constexpr OSThreadPriority() : mPriority(0) {}

        constexpr operator int() const { return mPriority; }

        constexpr friend bool operator<=>(const OSThreadPriority &, const OSThreadPriority &) = default;

        int mPriority; // off+0
    };

    inline constexpr Bedrock::Threading::OSThreadPriority High{2};
    inline constexpr Bedrock::Threading::OSThreadPriority Elevated{1};
    inline constexpr Bedrock::Threading::OSThreadPriority Normal{0};
    inline constexpr Bedrock::Threading::OSThreadPriority Low{-1};

} // namespace Bedrock::Threading