// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class FireAndForget
{
public:

    /**
     * execute task on fire and push to waiter tasks (with waiter)
     */
    template <typename Func>
    FireAndForget &operator>>(Func &&func)
    {
        futures_.push_back(run(std::forward<Func>(func)));
        return *this;
    }

    /**
     * execute task on fire (no waiter tasks), fast and simple
     */
    template <typename Func>
    FireAndForget &operator<<(Func &&func)
    {
        std::jthread([func = std::forward<Func>(func)]()
                     {
            try {
                if (std::uncaught_exceptions() > 0) {
                    throw std::runtime_error("Memory is being deallocated, stopping task.");
                }
                func();
            } catch (const std::exception& e) {
                std::cerr << "Exception in immediate task: " << e.what() << '\n';
            } catch (...) {
                std::cerr << "Unknown exception in immediate task.\n";
            } })
            .detach();

        return *this;
    }

    // اجرا کردن یک تسک و بازگشت future برای صبر کردن
    template <typename Func>
    std::future<void> run(Func &&func)
    {
        std::promise<void> promise;
        auto future = promise.get_future();

        std::jthread([func = std::forward<Func>(func), promise = std::move(promise)]() mutable
                     {
            try {
                func();
                promise.set_value(); 
            } catch (const std::exception& e) {
                std::cerr << "Exception in thread: " << e.what() << '\n';
                promise.set_exception(std::current_exception());
            } catch (...) {
                std::cerr << "Unknown exception in thread.\n";
                promise.set_exception(std::current_exception());
            } })
            .detach();

        return future;
    }

    // wait for one task
    void waitForTask(std::future<void> &future)
    {
        future.wait();
    }

    // wait for all tasks
    void waitForAllTasks()
    {
        for (auto &future : futures_)
        {
            future.wait();
        }
    }

private:
    std::vector<std::future<void>> futures_;
};

FireAndForget fire;


void fire_test()
{

    fire >> lm
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 1 executed\n";
    };

    fire >> lm
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 2 executed\n";
    };

    // اجرای فوری یک تسک با استفاده از عملگر <<
    fire << lm
    {
        // fire fire;
        fire << lm
        {
            std::cout << "Immediate Task executed\n";
            fire << lm
            {
                std::cout << "Immediate Task executed\n";
            };
        };
    };

    fire << lm
    {
        std::cout << "Immediate Task executed\n";
    };

    fire >> lm
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Task 3 executed\n";
    };

    // صبر کردن برای اتمام تسک‌ها
    std::cout << "Waiting for all tasks to complete...\n";
    fire.waitForAllTasks();

    std::cout << "All tasks completed.\n";

}
