#include <deque>
#include <format>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include "common/coroutine/Task.hpp"
#include "common/coroutine/SyncWait.hpp"
#include "common/coroutine/StaticThreadPool.hpp"
#include "common/coroutine/WhenAll.hpp"

#define TEST_CASE(name)                                                                                \
    static void test_##name();                                                                         \
    struct TestRegisterer_##name {                                                                     \
        TestRegisterer_##name() {                                                                      \
            try {                                                                                      \
                std::cout << std::format(">> Running test: " #name "\n");                              \
                test_##name();                                                                         \
                std::cout << std::format(">> Test " #name " passed \n\n");                             \
            } catch (std::exception & e) {                                                             \
                std::cout << std::format(">> Test " #name " Failed! \n>> e.what(): {}\n\n", e.what()); \
            }                                                                                          \
        }                                                                                              \
    } test_registerer_##name;                                                                          \
    static void test_##name()

using namespace sapphire::coro;

TEST_CASE(SimpleTask) {
    auto mul = [](int a, int b) -> Task<int> {
        co_return a *b;
    };
    auto task = [&mul]() -> Task<int> {
        int res = co_await mul(3, 2);
        co_return 1 + res;
    };

    if (syncWait(task()) != 7) {
        throw std::runtime_error("SimpleTask failed");
    }
}

TEST_CASE(VoidTask) {
    bool executed = false;
    auto void_task = [&]() -> Task<void> {
        executed = true;
        co_return;
    };
    syncWait(void_task());
    if (!executed) {
        throw std::runtime_error("VoidTask failed");
    }
}

TEST_CASE(ExceptionHandling) {
    auto exceptional_task = []() -> Task<int> {
        throw std::runtime_error("Exception from coroutine");
        co_return 0;
    };

    try {
        syncWait(exceptional_task());
        throw std::runtime_error("ExceptionHandling failed: no exception thrown");
    } catch (const std::runtime_error &e) {
        if (std::string(e.what()) != "Exception from coroutine") {
            throw std::runtime_error("ExceptionHandling failed: wrong exception message");
        }
    }
}

TEST_CASE(NestedTasks) {
    auto innerTask = [](int a, int b) -> Task<int> {
        co_return a + b;
    };

    auto middleTask = [&](int a, int b) -> Task<int> {
        int sum = co_await innerTask(a, b);
        co_return sum * 2;
    };

    auto outerTask = [&]() -> Task<int> {
        int result = co_await middleTask(3, 4);
        co_return result + 1;
    };

    if (syncWait(outerTask()) != 15) {
        throw std::runtime_error("NestedTasks failed");
    }
}

TEST_CASE(LValueReferenceTask) {
    int  value = 10;
    auto ref_task = [&]() -> Task<int &> {
        co_return value;
    };

    int &result = syncWait(ref_task());
    if (result != 10) {
        throw std::runtime_error("LValueReferenceTask failed: incorrect value");
    }

    result = 20;
    if (value != 20) {
        throw std::runtime_error("LValueReferenceTask failed: modification through reference failed");
    }
}

TEST_CASE(ConstLValueReferenceTask) {
    int  value = 10;
    auto ref_task = [&]() -> Task<const int &> {
        co_return value;
    };

    auto &result = syncWait(ref_task());
    if (result != 10) {
        throw std::runtime_error("ConstLValueReferenceTask failed: incorrect value");
    }

    const_cast<int &>(result) = 20;
    if (value != 20) {
        throw std::runtime_error("ConstLValueReferenceTask failed: modification through reference failed");
    }

    static_assert(std::is_same_v<decltype(result), const int &>, "Should be a const reference");
}

TEST_CASE(StringTask) {
    auto string_task = []() -> Task<std::string> {
        co_return "hello";
    };

    auto combined_task = [&]() -> Task<std::string> {
        std::string res = co_await string_task();
        co_return res + " world";
    };

    if (syncWait(combined_task()) != "hello world") {
        throw std::runtime_error("StringTask failed");
    }
}

struct Movable {
    Movable(int v) : value(v) {
        std::cout << std::format("  Movable({})\n", value);
    }
    Movable(const Movable &other) = delete;
    Movable(Movable &&other) noexcept : value(other.value) {
        other.value = -1;
        std::cout << std::format("  Movable move ctor({})\n", value);
    }
    ~Movable() { std::cout << std::format("  ~Movable({})\n", value); }
    int value;
};

TEST_CASE(StructLifecycleTask) {
    auto struct_task = []() -> Task<Movable> {
        std::cout << std::format("  Inside struct_task, before co_return\n");
        Movable res{42};
        co_return res;
    };
    std::cout << std::format("Calling syncWait for StructLifecycleTask...\n");
    Movable &&result = syncWait(struct_task()); // 预期两次移动构造
    std::cout << std::format("syncWait for StructLifecycleTask returned.\n");

    if (result.value != 42) {
        throw std::runtime_error("StructLifecycleTask failed: incorrect value");
    }
}

TEST_CASE(NestedTaskOfTask) {
    auto inner_task = []() -> Task<int> {
        co_return 100;
    };

    auto outer_task = [&]() -> Task<Task<int>> {
        co_return inner_task();
    };

    auto consuming_task = [&]() -> Task<int> {
        Task<int> t = co_await outer_task();
        int       result = co_await t;
        co_return result;
    };

    if (syncWait(consuming_task()) != 100) {
        throw std::runtime_error("NestedTaskOfTask failed");
    }
}

TEST_CASE(StaticThreadPoolTest) {
    constexpr size_t    num_threads = 4;
    constexpr size_t    num_tasks = 10000;
    StaticThreadPool    pool(num_threads);
    std::atomic<size_t> counter = 0;

    auto task_on_pool = [&]() -> Task<void> {
        co_await pool.schedule();
        // Now we are on a thread pool thread
        counter.fetch_add(1, std::memory_order_relaxed);
    };

    std::vector<Task<void>> tasks;
    for (size_t i = 0; i < num_tasks; ++i) {
        tasks.emplace_back(task_on_pool());
    }

    syncWait(whenAll(std::move(tasks)));

    if (counter.load() != num_tasks) {
        throw std::runtime_error(
            std::format("StaticThreadPoolTest failed: expected {}, got {}", num_tasks, counter.load())
        );
    }
}

TEST_CASE(StaticThreadPoolDivideAndConquerTest) {
    constexpr size_t num_threads = 4;
    constexpr size_t total_work_items = 20000;
    constexpr size_t base_case_threshold = 100; // When to stop dividing

    StaticThreadPool    pool(num_threads);
    std::atomic<size_t> counter = 0;

    // A forward declaration for the recursive task
    std::function<Task<void>(size_t, size_t)> process_range;

    process_range = [&](size_t start, size_t end) -> Task<void> {
        co_await pool.schedule();

        size_t count = end - start;
        if (count <= 0) {
            co_return;
        }

        if (count <= base_case_threshold) {
            // Base case: do the work directly
            counter.fetch_add(count, std::memory_order_relaxed);
        } else {
            // Recursive step: divide the work in two
            size_t mid = start + count / 2;
            // Create two child tasks that will likely run on different threads
            auto task1 = process_range(start, mid);
            auto task2 = process_range(mid, end);
            // Wait for both to complete
            co_await task1;
            co_await task2;
        }
    };

    // Start the whole process with a single task
    syncWait(process_range(0, total_work_items));

    if (counter.load() != total_work_items) {
        throw std::runtime_error(
            std::format("StaticThreadPoolDivideAndConquerTest failed: expected {}, got {}", total_work_items, counter.load())
        );
    }
}

TEST_CASE(StaticThreadPoolMixedLoadTest) {
    constexpr size_t num_threads = 8; // Use more threads for a more intense test
    constexpr size_t num_producers = 8;
    constexpr size_t tasks_per_producer = 5000;
    constexpr int    long_task_ratio = 10; // 1 in 10 tasks is a long task

    StaticThreadPool    pool(num_threads);
    std::atomic<size_t> total_tasks_completed = 0;

    auto short_task = [&]() -> Task<void> {
        co_await pool.schedule();
        total_tasks_completed.fetch_add(1, std::memory_order_relaxed);
    };

    auto long_task = [&]() -> Task<void> {
        co_await pool.schedule();
        // Simulate CPU-bound work
        volatile int dummy = 0;
        for (int i = 0; i < 10000; ++i) {
            dummy += i;
        }
        total_tasks_completed.fetch_add(1, std::memory_order_relaxed);
    };

    std::atomic<int> producerId{0};

    auto producer = [&]() {
        thread_local std::mt19937       gen(std::random_device{}());
        std::uniform_int_distribution<> distrib(1, 100);

        int    this_promise = producerId++;
        size_t pushedCount = 0;

        std::vector<Task<void>> tasks;
        for (size_t i = 0; i < tasks_per_producer; ++i) {
            if (distrib(gen) <= long_task_ratio) {
                tasks.emplace_back(long_task());
            } else {
                tasks.emplace_back(short_task());
            }
        }

        // Wait for all tasks submitted by this producer to complete
        syncWait([&]() -> Task<void> {
            for (size_t i = 0; i < tasks.size(); ++i) {
                co_await tasks[i];
            }
            std::cout << std::format(" producer done, coro: {}, cnt: {}\n", this_promise, pushedCount);
        }());
    };

    std::vector<std::jthread> producers;
    for (size_t i = 0; i < num_producers; ++i) {
        producers.emplace_back(producer);
    }

    for (auto &p : producers) {
        p.join();
    }

    constexpr size_t expected_tasks = num_producers * tasks_per_producer;
    if (total_tasks_completed.load() != expected_tasks) {
        throw std::runtime_error(
            std::format("StaticThreadPoolMixedLoadTest failed: expected {}, got {}", expected_tasks, total_tasks_completed.load())
        );
    }
}

TEST_CASE(WhenAllVariadic) {
    StaticThreadPool pool(4);
    std::atomic<int> counter = 0;

    auto task = [&](int i) -> Task<void> {
        co_await pool.schedule();
        counter.fetch_add(i, std::memory_order_relaxed);
    };

    syncWait(whenAll(task(1), task(2), task(3)));

    if (counter.load() != 6) {
        throw std::runtime_error("WhenAllVariadic failed");
    }
}

TEST_CASE(WhenAllVector) {
    StaticThreadPool       pool(4);
    std::atomic<int>       counter = 0;
    std::vector<Task<int>> tasks;

    auto task = [](int i, StaticThreadPool &pool, std::atomic<int> &counter) -> Task<int> {
        co_await pool.schedule();
        counter.fetch_add(i, std::memory_order_relaxed);
        co_return i;
    };

    tasks.emplace_back(task(1, pool, counter));
    tasks.emplace_back(task(2, pool, counter));
    tasks.emplace_back(task(3, pool, counter));

    auto results = syncWait(whenAll(std::move(tasks)));
    if (results.size() != 3) {
        throw std::runtime_error("WhenAllVector failed: incorrect results size");
    }
    if (results[0].result() != 1) {
        throw std::runtime_error("WhenAllVector failed: incorrect return value: task[0] != 1");
    }
    if (results[1].result() != 2) {
        throw std::runtime_error("WhenAllVector failed: incorrect return value: task[1] != 2");
    }
    if (results[2].result() != 3) {
        throw std::runtime_error("WhenAllVector failed: incorrect return value: task[2] != 3");
    }

    if (counter.load() != 6) {
        throw std::runtime_error("WhenAllVector failed");
    }
}

int main() {
    return 0;
}