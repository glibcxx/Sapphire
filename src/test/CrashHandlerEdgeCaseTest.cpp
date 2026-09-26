#include <iostream>
#include <string_view>
#include <thread>
#include <vector>
#include "common/debug/CrashHandler.hpp"

#pragma init_seg(lib)
static sapphire::debug::CrashHandler handler;

// Case 1: 极早期崩溃
// 全局变量构造函数在进入 main() 前解引用 nullptr
struct EarlyCrashingGlobal {
    EarlyCrashingGlobal(bool trigger) {
        if (trigger) {
            std::cout << "[Pre-main] Crashing inside global constructor before main()...\n";
            volatile int *p = nullptr;
            *p = 0xBAD1;
        }
    }
};

EarlyCrashingGlobal gEarlyObj(true); // 取消注释可直接验证 Pre-main

// Case 2: 极晚期崩溃
// main() 结束后，静态析构函数中解引用已释放内存
struct LateCrashingGlobal {
    bool mArm = false;
    ~LateCrashingGlobal() {
        if (mArm) {
            std::cout << "[Post-main] Crashing inside global destructor after main()...\n";
            volatile int *p = nullptr;
            *p = 0xBAD2;
        }
    }
};
LateCrashingGlobal gLateObj;

// Case 3: 多线程雪崩崩溃
// 10 个线程同时解引用同一个野指针
void triggerCrashStorm() {
    std::cout << "Triggering Multi-threaded Crash Storm (10 threads crashing simultaneously)...\n";
    constexpr int            THREAD_COUNT = 10;
    std::vector<std::thread> workers;
    volatile bool            startGate = false;

    for (int i = 0; i < THREAD_COUNT; ++i) {
        workers.emplace_back([&startGate, i]() {
            while (!startGate) {
                _mm_pause(); // 忙等待对齐启动时间
            }
            volatile int *p = nullptr;
            *p = 0xDEAD0000 + i; // 几乎在同一微秒全部崩溃
        });
    }

    startGate = true; // 放开栅栏
    for (auto &t : workers) {
        if (t.joinable()) t.join();
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: test-CrashHandlerEdgeCaseTest.exe <option>\n"
                  << "  --late          : Test Post-main (global destructor crash)\n"
                  << "  --storm         : Test Concurrent Multi-threaded Crash\n";
        return 0;
    }

    std::string_view opt = argv[1];

    if (opt == "--late") {
        gLateObj.mArm = true;
        std::cout << "Armed late crash. Exiting main()...\n";
        return 0;
    } else if (opt == "--storm") {
        triggerCrashStorm();
    }

    return 0;
}