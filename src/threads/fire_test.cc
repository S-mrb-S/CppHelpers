#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class FireAndForget
{
public:
    FireAndForget() : active_tasks_(0), stop_pool_(false)
    {
        // Start a pool of worker threads
        for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex_);
                        cond_var_.wait(lock, [this] { return stop_pool_ || !tasks_.empty(); });
                        if (stop_pool_ && tasks_.empty())
                            return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();  // Execute the task
                }
            });
        }
    }

    ~FireAndForget()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            stop_pool_ = true;
        }
        cond_var_.notify_all();
        for (std::thread &worker : workers_)
            worker.join();
    }

    template <typename Func>
    FireAndForget &operator<<(Func &&func)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            tasks_.emplace([this, func = std::forward<Func>(func)]() {
                execute(func);
            });
        }
        cond_var_.notify_one();  // Notify one waiting worker thread
        return *this;
    }

    void waitForAllTasks()
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        cond_var_.wait(lock, [this] { return tasks_.empty(); });
    }

private:
    void execute(const std::function<void()> &func)
    {
        ++active_tasks_;  // Increment active task count
        try {
            func();  // Execute the function
        } catch (const std::exception &e) {
            std::cerr << "Exception in task: " << e.what() << '\n';
        } catch (...) {
            std::cerr << "Unknown exception in task.\n";
        }
        --active_tasks_;  // Decrement active task count
    }

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable cond_var_;
    std::atomic<int> active_tasks_; // Keep track of active tasks
    bool stop_pool_;
};

FireAndForget fire;

void fire_test()
{
    fire << [] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 1 executed\n";
    };

    fire << [] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 2 executed\n";
    };

    fire << [] {
        std::cout << "Immediate Task executed\n";
    };

    fire << [] {
        std::cout << "Immediate Task executed\n";
    };

    fire << [] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Task 3 executed\n";
    };

    std::cout << "Waiting for all tasks to complete...\n";
    // fire.waitForAllTasks();
    std::cout << "All tasks completed.\n";
}

// Main function
int main() {
    fire_test();
    return 0;
}
