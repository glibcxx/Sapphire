#include <iostream>
#include "common/debug/StackTrace.hpp"

using namespace sapphire::debug;

[[msvc::noinline]] void functionC() {
    std::cout << "\n--- [Test 1: One-Liner currentToString] ---\n";
    // 期望第 [ 0] 帧直接指向 functionC
    std::cout << StackTrace::currentToString();

    std::cout << "\n--- [Test 2: Container Iteration & Lazy Properties] ---\n";
    auto trace = StackTrace::current();
    for (size_t i = 0; i < (std::min)(trace.size(), size_t(4)); ++i) {
        const auto &frame = trace[i];
        std::cout << "Frame #" << i
                  << " | Addr: 0x" << std::hex << frame.address() << std::dec
                  << " | Func: " << frame.functionName()
                  << " | File: " << frame.sourceFile()
                  << ":" << frame.sourceLine() << "\n";
    }

    std::cout << "\n--- [Test 3: Zero-Alloc walkCurrent (Early Break)] ---\n";
    size_t count = 0;
    StackTrace::walkCurrent([&count](const StackFrame &frame) {
        std::cout << "Walked frame: 0x" << std::hex << frame.address() << std::dec << "\n";
        return ++count < 2; // 只取前 2 帧即提前 break
    });

    std::cout << "\n--- [Test 4: fromContext using RtlCaptureContext] ---\n";
    CONTEXT ctx{};
    RtlCaptureContext(&ctx);
    auto ctxTrace = StackTrace::fromContext(ctx);
    std::cout << "Context Trace Size: " << ctxTrace.size() << "\n";
    std::cout << ctxTrace.toString(4);
}

[[msvc::noinline]] void functionB() {
    volatile int antiTcoMarker = 1;
    functionC();
    antiTcoMarker = antiTcoMarker + 1;
}

[[msvc::noinline]] void functionA() {
    volatile int antiTcoMarker = 2;
    functionB();
    antiTcoMarker = antiTcoMarker + 1;
}

int main() {
    functionA();
    return 0;
}