#include <exception>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <thread>

#include "common/debug/CrashHandler.hpp"

using sapphire::debug::CrashHandler;

// ============================================================================
// 1. SEH 异常测试用例
// ============================================================================

// 1.1 非法内存访问：写入空指针 (Access Violation - 0xC0000005)
void TestAccessViolationWrite() {
    std::cout << "[Trigger] Access Violation (Write null)...\n";
    volatile int *ptr = nullptr;
    *ptr = 0xDEAD; // 触发写入非法内存
}

// 1.2 非法内存访问：读取非法地址
void TestAccessViolationRead() {
    std::cout << "[Trigger] Access Violation (Read wild pointer)...\n";
    volatile int *ptr = reinterpret_cast<int *>(0x12345678);
    volatile int  val = *ptr; // 触发读取非法内存
    (void)val;
}

// 1.3 整数除以零 (Divide by Zero - 0xC0000094)
void TestDivideByZero() {
    std::cout << "[Trigger] Integer Divide by Zero...\n";
    volatile int zero = 0;
    volatile int result = 100 / zero;
    (void)result;
}

inline volatile bool g_ForceStopRecursion = false;

#pragma optimize("", off)
__declspec(noinline) void RecursiveOverflowImpl(int depth) {
    volatile char frameBuffer[4096];
    frameBuffer[0] = static_cast<char>(depth);
    if (g_ForceStopRecursion) {
        return;
    }
    RecursiveOverflowImpl(depth + 1);
    frameBuffer[sizeof(frameBuffer) - 1] = static_cast<char>(depth);
}

void TestStackOverflow() {
    std::cout << "[Trigger] Stack Overflow...\n";
    void (*volatile pFunc)(int) = RecursiveOverflowImpl;
    pFunc(0);
}
#pragma optimize("", on)

// ============================================================================
// 2. C++ 与 CRT 运行时异常测试用例
// ============================================================================

// 2.1 未捕获的 C++ 异常
void TestUnhandledCppException() {
    std::cout << "[Trigger] Unhandled C++ Exception...\n";
    std::thread{[] {
        throw std::runtime_error("Simulated unhandled std::runtime_error");
    }}.join();
}

// 2.2 纯虚函数调用 (_purecall)
// 利用构造期间多态未绑定的特点，间接触发纯虚函数调用
struct BaseTest {
    BaseTest() { CallPure(); }
    void         CallPure() { PureVirtualFunction(); }
    virtual void PureVirtualFunction() = 0;
};
struct DerivedTest : public BaseTest {
    void PureVirtualFunction() override {}
};
void TestPureVirtualCall() {
    std::cout << "[Trigger] Pure Virtual Function Call...\n";
    DerivedTest d; // Base 构造时调用 PureVirtualFunction 会触发 CRT purecall
}

// 2.3 CRT 非法参数检查失败 (Invalid Parameter)
void TestInvalidParameter() {
    std::cout << "[Trigger] CRT Invalid Parameter (strcpy_s buffer too small)...\n";
    char dest[4];
    // 故意拷贝超出目标容量的字符串，触发 CRT 严格安全检查
    strcpy_s(dest, 4, "Too long string for buffer");
}

// 2.4 主动调用 abort() 与信号
void TestAbortSignal() {
    std::cout << "[Trigger] std::abort() / SIGABRT...\n";
    std::abort();
}

// ============================================================================
// 3. 用户态主动引发 SEH 异常
// ============================================================================
void TestCustomRaiseException() {
    std::cout << "[Trigger] Custom RaiseException (0xE0000001)...\n";
    RaiseException(0xE0000001, EXCEPTION_NONCONTINUABLE, 0, nullptr);
}

int main(int argc, char *argv[]) {
    CrashHandler instance;

    std::cout << "========================================\n";
    std::cout << "       CrashHandler Test Suite          \n";
    std::cout << "========================================\n";
    std::cout << "1. Access Violation (Write null)\n";
    std::cout << "2. Access Violation (Read wild address)\n";
    std::cout << "3. Integer Divide by Zero\n";
    std::cout << "4. Stack Overflow\n";
    std::cout << "5. Unhandled C++ Exception (std::throw)\n";
    std::cout << "6. CRT Pure Virtual Function Call\n";
    std::cout << "7. CRT Invalid Parameter Handler\n";
    std::cout << "8. std::abort() (SIGABRT)\n";
    std::cout << "9. Custom RaiseException\n";
    std::cout << "0. Exit\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Select test case [0-9]: ";

    int choice = -1;
    if (!(std::cin >> choice)) {
        return 0;
    }

    switch (choice) {
    case 1: TestAccessViolationWrite(); break;
    case 2: TestAccessViolationRead(); break;
    case 3: TestDivideByZero(); break;
    case 4: TestStackOverflow(); break;
    case 5: TestUnhandledCppException(); break;
    case 6: TestPureVirtualCall(); break;
    case 7: TestInvalidParameter(); break;
    case 8: TestAbortSignal(); break;
    case 9: TestCustomRaiseException(); break;
    default:
        std::cout << "Exiting without crash.\n";
        break;
    }

    return 0;
}