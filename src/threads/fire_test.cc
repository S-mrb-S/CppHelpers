#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <atomic>

class FireAndForget
{
public:
    FireAndForget() : active_tasks_(0) {}

    /**
     * Execute task on fire and push to waiter tasks (with waiter)
     */
    template <typename Func>
    FireAndForget &operator>>(Func &&func)
    {
        futures_.emplace_back(run(std::forward<Func>(func)));
        return *this;
    }

    /**
     * Execute task on fire (no waiter tasks), fast and simple
     */
    template <typename Func>
    FireAndForget &operator<<(Func &&func)
    {
        std::jthread([this, func = std::forward<Func>(func)]() mutable {
            execute(func);
        }).detach();  // Detach since we don't wait for this task.

        return *this;
    }

    // Run a task and return a future for waiting
    template <typename Func>
    std::future<void> run(Func &&func)
    {
        std::promise<void> promise;
        auto future = promise.get_future();
        
        std::jthread([this, func = std::forward<Func>(func), promise = std::move(promise)]() mutable {
            execute([&] { func(); promise.set_value(); });
        }).detach();

        return future;
    }

    // Wait for one task
    void waitForTask(std::future<void> &future)
    {
        future.wait();
    }

    // Wait for all tasks
    void waitForAllTasks()
    {
        for (auto &future : futures_)
        {
            future.wait();
        }
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

    std::vector<std::future<void>> futures_;
    std::atomic<int> active_tasks_; // Keep track of active tasks
};

FireAndForget fire;

int main()
{
    fire >> [] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 1 executed\n";
    };

    fire >> [] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 2 executed\n";
    };

    fire << [] {
        std::cout << "Immediate Task 1 executed\n";
    };

    fire << [] {
        std::cout << "Immediate Task executed\n";
    };

    fire >> [] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Task 3 executed\n";
    };

    std::cout << "Waiting for all tasks to complete...\n";
    fire.waitForAllTasks();
    std::cout << "All tasks completed.\n";

    return 0;
}
