#include <iostream>
#include <vector>
#include <cassert>
#include <windows.h>

#include "common/debug/Symbolizer.hpp"

using namespace sapphire::debug;

__declspec(noinline) int targetFunctionForTest(int a, int b) {
    volatile int x = a + b;
    return x;
}

namespace TestNamespace {
    class DummyService {
    public:
        __declspec(noinline) void executeWorker() {
            volatile int dummy = 42;
            (void)dummy;
        }
    };
} // namespace TestNamespace

void printSeparator(const char *title) {
    std::cout << "\n================ [ " << title << " ] ================\n";
}

int main() {
    auto &symbolizer = Symbolizer::instance();

    // ------------------------------------------------------------------------
    // Test 1: 初始化状态检查
    // ------------------------------------------------------------------------
    printSeparator("Test 1: Initialization Check");
    std::cout << "Symbolizer initialized: " << (symbolizer.isInitialized() ? "YES" : "NO") << "\n";
    assert(symbolizer.isInitialized() && "Symbolizer failed to initialize!");

    // ------------------------------------------------------------------------
    // Test 2: 本地函数与源码行号反查 (需生成 PDB)
    // ------------------------------------------------------------------------
    printSeparator("Test 2: Local Symbol & Line Number");
    uintptr_t funcAddr = reinterpret_cast<uintptr_t>(&targetFunctionForTest);

    SymbolInfo info{};
    bool       resolved = symbolizer.resolve(funcAddr, info);

    std::cout << "Target Addr      : 0x" << std::hex << funcAddr << std::dec << "\n";
    std::cout << "Resolve Success  : " << (resolved ? "YES" : "NO") << "\n";
    std::cout << "Module Name      : " << (info.mHasModule ? info.mModuleName : "<none>") << "\n";
    std::cout << "Function Name    : " << (info.mHasSymbol ? info.mFunctionName : "<none>") << "\n";
    std::cout << "Source File      : " << (info.mHasLine ? info.mFileName : "<none>") << "\n";
    std::cout << "Line Number      : " << (info.mHasLine ? std::to_string(info.mLineNumber) : "<none>") << "\n";
    std::cout << "Formatted Output : " << symbolizer.format(info) << "\n";

    // ------------------------------------------------------------------------
    // Test 3: 命名空间与类成员函数解析
    // ------------------------------------------------------------------------
    printSeparator("Test 3: Member Function Resolution");
    TestNamespace::DummyService dummy;
    auto                        memberFuncPtr = &TestNamespace::DummyService::executeWorker;
    uintptr_t                   memberAddr = 0;
    std::memcpy(&memberAddr, &memberFuncPtr, sizeof(uintptr_t));

    std::cout << "Resolved String  : " << symbolizer.resolveAndFormat(memberAddr) << "\n";

    // ------------------------------------------------------------------------
    // Test 4: 系统 DLL 导出函数解析 (无源码行号，仅有模块与导出名)
    // ------------------------------------------------------------------------
    printSeparator("Test 4: External DLL Symbol Resolution");
    HMODULE   hKernel32 = GetModuleHandleW(L"kernel32.dll");
    uintptr_t apiAddr = reinterpret_cast<uintptr_t>(GetProcAddress(hKernel32, "GetCurrentProcessId"));

    SymbolInfo apiInfo{};
    symbolizer.resolve(apiAddr, apiInfo);
    std::cout << "API Target Addr  : 0x" << std::hex << apiAddr << std::dec << "\n";
    std::cout << "Module (Expect kernel32/kernelbase): " << apiInfo.mModuleName << "\n";
    std::cout << "Symbol (Expect GetCurrentProcessId): " << apiInfo.mFunctionName << "\n";
    std::cout << "Formatted Output : " << symbolizer.format(apiInfo) << "\n";

    // ------------------------------------------------------------------------
    // Test 5: 批量解析测试 (std::span)
    // ------------------------------------------------------------------------
    printSeparator("Test 5: Batch Resolution (std::span)");
    std::vector<uintptr_t> addressList = {
        funcAddr,
        memberAddr,
        apiAddr,
        reinterpret_cast<uintptr_t>(&main)
    };

    std::vector<SymbolInfo> batchInfos(addressList.size());
    symbolizer.resolveBatch(addressList, batchInfos);

    for (size_t i = 0; i < batchInfos.size(); ++i) {
        std::cout << "  [" << i << "] " << symbolizer.format(batchInfos[i]) << "\n";
    }

    // ------------------------------------------------------------------------
    // Test 6: Demangle 符号反修饰测试
    // ------------------------------------------------------------------------
    printSeparator("Test 6: Demangle Test");
    // MSVC 针对 "int __cdecl AddNumbers(int, int)" 的修饰名
    std::string_view mangledSymbol = "?AddNumbers@@YAHHH@Z";

    std::string completeSig = Symbolizer::demangle(mangledSymbol, Symbolizer::DemangleOption::Complete);
    std::string nameOnly = Symbolizer::demangle(mangledSymbol, Symbolizer::DemangleOption::NameOnly);
    std::string noCalling = Symbolizer::demangle(mangledSymbol, Symbolizer::DemangleOption::NoCallingConv);

    std::cout << "Original Mangled : " << mangledSymbol << "\n";
    std::cout << "Complete         : " << completeSig << "\n";
    std::cout << "Name Only        : " << nameOnly << "\n";
    std::cout << "No Calling Conv  : " << noCalling << "\n";

    char stackBuf[128] = {};
    bool demangleOk = Symbolizer::demangle(mangledSymbol, std::span<char>(stackBuf), Symbolizer::DemangleOption::Complete);
    std::cout << "Span Demangle    : " << (demangleOk ? stackBuf : "FAILED") << "\n";

    // ------------------------------------------------------------------------
    // Test 7: Format 测试
    // ------------------------------------------------------------------------
    printSeparator("Test 7: Fallback Formatting Verification");

    // 1: 完整符号与代码行
    SymbolInfo l1{};
    l1.mAddress = 0x140001000;
    l1.mHasModule = true;
    strcpy_s(l1.mModuleName, "Engine.exe");
    l1.mHasSymbol = true;
    strcpy_s(l1.mFunctionName, "Renderer::Draw");
    l1.mFunctionOffset = 0x24;
    l1.mHasLine = true;
    strcpy_s(l1.mFileName, "C:\\Source\\Renderer.cpp");
    l1.mLineNumber = 128;
    std::cout << "Level 1 (All)    : " << symbolizer.format(l1) << "\n";

    // 2: 无行号（只有函数名和模块）
    SymbolInfo l2 = l1;
    l2.mHasLine = false;
    std::cout << "Level 2 (No Line): " << symbolizer.format(l2) << "\n";

    // 3: 无符号（只有模块名与 RVA，如第三方无 pdb 的 DLL）
    SymbolInfo l3{};
    l3.mAddress = 0x7FFF12345678;
    l3.mHasModule = true;
    strcpy_s(l3.mModuleName, "user32.dll");
    l3.mModuleOffset = 0x4A10;
    std::cout << "Level 3 (Module) : " << symbolizer.format(l3) << "\n";

    // 4: 完全未知地址
    SymbolInfo l4{};
    l4.mAddress = 0xDEADBEEF;
    std::cout << "Level 4 (Raw PC) : " << symbolizer.format(l4) << "\n";

    printSeparator("All Tests Finished");
    return 0;
}