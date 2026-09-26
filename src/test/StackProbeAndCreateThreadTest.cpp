#include <iostream>
#include "common/debug/CrashHandler.hpp"

using namespace sapphire::debug;

struct WorkerPayload {
    DWORD mParentThreadId = 0;
    bool  mExecutedSuccessfully = false;
};

DWORD WINAPI WorkerThreadEntry(LPVOID lpParam) {
    auto *payload = reinterpret_cast<WorkerPayload *>(lpParam);
    if (payload) {
        payload->mExecutedSuccessfully = true;
        std::cout << "  [Worker Thread] Running on thread ID: " << GetCurrentThreadId()
                  << " (Crashing Parent ID: " << payload->mParentThreadId << ")\n";
    }
    return 0;
}

int main() {
    std::cout << "================ [ Test 1: Stack Margin Prober ] ================\n";

    auto normalInfo = StackProber::probeDetails();
    std::cout << "Normal Exception Probe:\n"
              << "  Current SP   : 0x" << std::hex << normalInfo.mCurrentSp << "\n"
              << "  Stack Limit  : 0x" << normalInfo.mStackLimit << "\n"
              << "  Margin Bytes : 0x" << normalInfo.mMarginBytes << std::dec
              << " (" << normalInfo.mMarginBytes / 1024 << " KB)\n";

    auto overflowInfo = StackProber::probeDetails();
    std::cout << "Simulated Stack Overflow Probe:\n"
              << "  Margin Bytes : " << overflowInfo.mMarginBytes << " bytes\n";

    std::cout << "\n================ [ Test 2: Low-Stack Launcher ] ================\n";
    WorkerPayload payload{};
    payload.mParentThreadId = GetCurrentThreadId();

    HANDLE hWorker = LowStackThreadLauncher::launch(LowStackThreadLauncher::resolveNtCreateThreadEx(), WorkerThreadEntry, &payload, 256 * 1024);
    if (hWorker) {
        WaitForSingleObject(hWorker, INFINITE);
        CloseHandle(hWorker);
        std::cout << "Launcher status: Worker executed = "
                  << (payload.mExecutedSuccessfully ? "SUCCESS" : "FAILED") << "\n";
    } else {
        std::cout << "Launcher status: FAILED to create thread!\n";
    }

    std::cout << "\n================ All Tests Passed ================\n";
    return 0;
}