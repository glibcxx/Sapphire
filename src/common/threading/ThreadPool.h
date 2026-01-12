#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <concepts>

namespace sapphire {

    class ThreadPool {
    public:
        explicit ThreadPool(size_t num_threads = std::thread::hardware_concurrency()) :
            stop_(true), busy_workers_(0), num_threads_(num_threads ? num_threads : 1) {
        }

        ~ThreadPool() {
            {
                std::unique_lock lock(queue_mutex_);
                stop_ = true;
            }
            cv_.notify_all();
            for (std::thread &worker : workers_) {
                worker.join();
            }
        }

        ThreadPool(const ThreadPool &) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;
        ThreadPool(ThreadPool &&) = delete;
        ThreadPool &operator=(ThreadPool &&) = delete;

        void start() {
            std::unique_lock lock(queue_mutex_);
            if (!stop_) return;

            stop_ = false;
            for (size_t i = 0; i < num_threads_; ++i) {
                workers_.emplace_back([this] { worker_loop(); });
            }
        }

        void stop() {
            {
                std::unique_lock lock(queue_mutex_);
                stop_ = true;
            }
            cv_.notify_all();
            for (std::thread &worker : workers_) {
                worker.join();
            }
            workers_.clear();
        }

        template <std::invocable F, typename... Args>
        auto enqueue(F &&f, Args &&...args) -> std::future<std::invoke_result_t<F, Args...>> {
            using ReturnType = std::invoke_result_t<F, Args...>;

            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                [f = std::forward<F>(f), ... args = std::forward<Args>(args)]() mutable {
                    return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
                }
            );

            std::future<ReturnType> future = task->get_future();
            {
                std::unique_lock lock(queue_mutex_);
                tasks_.emplace([task]() { (*task)(); });
            }
            cv_.notify_one();
            return future;
        }

        void wait_all_finished() {
            std::unique_lock lock(queue_mutex_);
            wait_cv_.wait(lock, [this] { return this->tasks_.empty() && this->busy_workers_ == 0; });
        }

    private:
        void worker_loop() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock lock(queue_mutex_);
                    cv_.wait(lock, [this] { return this->stop_ || !this->tasks_.empty(); });

                    if (this->stop_ && this->tasks_.empty()) {
                        return;
                    }

                    task = std::move(tasks_.front());
                    tasks_.pop();

                    busy_workers_++;
                }

                task();

                {
                    std::unique_lock lock(queue_mutex_);
                    busy_workers_--;

                    if (this->busy_workers_ == 0 && this->tasks_.empty()) {
                        wait_cv_.notify_all();
                    }
                }
            }
        }

        size_t                            num_threads_;
        std::vector<std::thread>          workers_;
        std::queue<std::function<void()>> tasks_;
        std::mutex                        queue_mutex_;
        std::condition_variable           cv_;
        bool                              stop_;

        size_t                  busy_workers_;
        std::condition_variable wait_cv_;
    };

} // namespace util