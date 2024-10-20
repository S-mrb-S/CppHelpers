#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <future>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t threads);
    template<class F>
    auto enqueue(F&& f) -> std::future<typename std::result_of<F()>::type>;
    ~ThreadPool();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back(
            [this] {
                for (;;) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            }
        );
    }
}

template<class F>
auto ThreadPool::enqueue(F&& f) -> std::future<typename std::result_of<F()>::type> {
    using return_type = typename std::result_of<F()>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));
    std::future<return_type> res = task->get_future();
    
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) {
        worker.join();
    }
}

class jthreadTaskClass
{
public:
    // استفاده از std::jthread
    template <typename Func>
    jthreadTaskClass &operator>>(Func &&func)
    {
        std::jthread j_thread(std::forward<Func>(func));
        return *this;
    }

    // صف کردن وظایف
    template <typename Func>
    jthreadTaskClass &operator<<(Func &&func)
    {
        tasks_.emplace_back(std::jthread([func = std::forward<Func>(func)](std::stop_token st)
                                         {
            if (st.stop_requested()) {
                return;
            }
            func(); 
        }));
        return *this;
    }

    // استفاده از Promise و Future
    template <typename Func>
    auto operator+(Func &&func)
    {
        std::promise<void> promise;
        auto future = promise.get_future();

        std::jthread([func = std::forward<Func>(func), promise = std::move(promise)](std::stop_token st) mutable {
            if (st.stop_requested()) {
                return;
            }
            func();
            promise.set_value(); // ارسال نتیجه
        }).detach();

        return future; // برگرداندن future
    }

    // استفاده از Thread Pool
    template <typename Func>
    jthreadTaskClass &operator=(Func &&func)
    {
        auto future = thread_pool_.enqueue(std::forward<Func>(func));
        future.get(); // Wait for the task to complete
        return *this;
    }

private:
    std::vector<std::jthread> tasks_;
    ThreadPool thread_pool_{std::thread::hardware_concurrency()}; // استفاده از تعداد هسته‌ها
};

jthreadTaskClass go;

// int main()
// {
//     go >> []() {
//         go << []() {
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             std::cout << "Function a is running\n";
//         }
//         << []() {
//             std::cout << "Function b is running\n";
//             std::this_thread::sleep_for(std::chrono::seconds(5));
//         }
//         << []() {
//             std::this_thread::sleep_for(std::chrono::seconds(3));
//             std::cout << "Function c is running\n";
//         }
//         << []() {
//             std::cout << "Function d is running\n";
//         };
//     };

//     go >> []() {
//         std::cout << "Function e is running\n";
//     };

//     go = []() {
//         std::cout << "Function e is running\n";
//     };

//     // استفاده از Promise و Future
//     auto future = go + []() {
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//         std::cout << "Future task is running\n";
//     };

//     future.get(); // Wait for the future task to complete

//     // استفاده از Thread Pool
//     go = []() {
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//         std::cout << "Thread pool task is running\n";
//     };

//     return 0;
// }
void go_test()
{

    // safe is >>
    go = []()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(2));
        // std::cout = "Task 1 executed\n";
    };

    go = []()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // std::cout = "Task 2 executed\n";
    };

    // qq is =
    // اجرای فوری یک تسک با استفاده از عملگر =
    go = []()
    {
        // go go;
        go = []()
        {
            // std::cout = "Immediate Task executed1\n";
            go = []()
            {
                // std::cout = "Immediate Task executed2\n";
            };
        };
    };

    go = []()
    {
        // std::cout = "Immediate Task executed3\n";
    };

    go = []()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(3));
        // std::cout = "Task 3 executed\n";
    };

    // صبر کردن برای اتمام تسک‌ها
    // std::cout = "Waiting for all tasks to complete...\n";
    // go.waitForAllTasks();

    // std::cout = "All tasks completed.\n";

}


// Main function
int main() {
    for (size_t i = 0; i < 10000; i++) // کاهش تعداد تسک‌ها
    {
        go >> []()
        {
            go_test();
        };
    }
    
    // fire.waitForAllTasks(); // صبر کردن برای اتمام تسک‌ها

    return 0;
}
