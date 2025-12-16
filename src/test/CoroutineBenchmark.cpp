#include <format>
#include <iostream>

#include "common/coroutine/StaticThreadPool.hpp"
#include "common/coroutine/Task.hpp"
#include "common/coroutine/SyncWait.hpp"
#include "common/coroutine/WhenAll.hpp"
#include "common/ScopedTimer.hpp"

using namespace sapphire::coro;

// 不断地将自己重新调度回线程池，测试 "Context Switch" 开销
Task<void> runSchedulerBench(StaticThreadPool &pool, size_t iterations) {
    co_await pool.schedule();

    for (size_t i = 0; i < iterations; ++i) {
        co_await pool.schedule();
    }
}

void benchLatency(StaticThreadPool &pool) {
    std::cout << "\n[Running Latency Benchmark]...\n";

    const size_t iterations = 100'000'000; // 1亿次切换

    // 预热
    syncWait(runSchedulerBench(pool, 10000));

    util::TimerToken token;
    {
        util::ScopedTimer timer{token};
        syncWait(runSchedulerBench(pool, iterations));
    }

    double totalNs = token.getDurationNs();

    std::cout << std::format(
        "  Iterations: {}\n"
        "  Total Time: {:.4f} ms\n"
        "  -> Latency: {:.4f} ns/op (Lower is better)\n",
        iterations,
        token.getDurationMs(),
        totalNs / iterations
    );
}

// 启动 N 个外部线程，每个线程提交一个长链任务。
// 这模拟了所有 Worker 满载，且 Global Queue 和 Local Queue 都在高频工作的场景。
void benchThroughput(StaticThreadPool &pool) {
    std::cout << "\n[Running Throughput Benchmark]...\n";

    size_t threadCount = std::thread::hardware_concurrency();
    // 增加并发度以确保填满所有核心并触发 Stealing
    size_t concurrentChains = threadCount * 2;

    const size_t opsPerChain = 50'000'000;
    const size_t totalOps = concurrentChains * opsPerChain;

    std::vector<Task<>> tasks;
    tasks.reserve(concurrentChains);

    util::TimerToken token;
    {
        util::ScopedTimer timer{token};
        for (size_t i = 0; i < concurrentChains; ++i) {
            tasks.emplace_back([](StaticThreadPool &pool) -> Task<> {
                co_await pool.schedule();
                co_await runSchedulerBench(pool, opsPerChain);
            }(pool));
        }
        syncWait(whenAll(std::move(tasks)));
    }

    double totalMS = token.getDurationMs();
    double opsPerSec = totalOps / (totalMS / 1000.0);

    std::cout << std::format(
        "  Concurrency: {}\n"
        "  Total Ops: {}\n"
        "  Total Time: {:.4f} ms\n"
        "  -> Throughput: {:.4f} M ops/sec (Higher is better)\n",
        concurrentChains,
        totalOps,
        totalMS,
        (opsPerSec / 1'000'000.0)
    );
}

int main() {
    std::cout << "=== Coroutine ThreadPool Benchmark ===\n";
    std::cout << "Hardware Concurrency: " << std::thread::hardware_concurrency() << '\n';

    StaticThreadPool pool;

    benchLatency(pool);
    benchThroughput(pool);

    return 0;
}