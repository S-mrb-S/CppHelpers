// #include <iostream>
// #include <chrono>
// #include <thread>
// #include <vector>
// #include <functional>
// #include <any>

// class cthread {
// public:
//     // سازنده پیش‌فرض
//     cthread() = default;

//     // عملگر هم‌ارزی برای اضافه کردن توابع لامبدا
//     template<typename Func>
//     cthread& operator<<(Func&& func) {
//         // ایجاد یک شی جدید jthread و اضافه کردن آن به لیست
//         tasks_.emplace_back(std::jthread([func = std::forward<Func>(func)](std::stop_token stoken) mutable {
//             // بررسی وضعیت توقف و اجرا
//             if (stoken.stop_requested()) return;
//             func();
//         }));
//         return *this; // برگشت این شی برای زنجیره‌ای کردن
//     }

// private:
//     std::vector<std::jthread> tasks_; // وکتور برای ذخیره وظایف
// };

// #define go cthread() <<

// std::any myFun(){
//     std::this_thread::sleep_for(std::chrono::seconds(2)); // شبیه‌سازی کار
//     std::cout << "Function a is running\n";

//     return nullptr;
// }

// int main() {
//     // cthread task; // ایجاد یک شی go

//     // استفاده از عملگر << برای اضافه کردن توابع لامبدا
//     go []() {
//         std::this_thread::sleep_for(std::chrono::seconds(2)); // شبیه‌سازی کار
//         std::cout << "Function a is running\n";
//     };

//     return 0;
// }

// #include <iostream>
// #include <thread>
// #include <chrono>
// #include <vector>

// class jthreadTaskClass
// {
// public:
//     template <typename Func>
//     jthreadTaskClass &operator>>(Func &&func)
//     {
//         std::jthread j_thread(std::forward<Func>(func));
//         return *this;
//     }

//     template <typename Func>
//     jthreadTaskClass &operator<<(Func &&func)
//     {
//         tasks_.emplace_back(std::jthread([func = std::forward<Func>(func)](std::stop_token st)
//                                          {
//             if (st.stop_requested()) {
//                 return;
//             }
//             func(); }));
//         return *this;
//     }

// private:
//     std::vector<std::jthread> tasks_;
// };

// jthreadTaskClass go;

// int main()
// {
//     go >> []() {
//         go << []()
//         {
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             std::cout << "Function a is running\n";
//         }
//         << []()
//         {
//             std::cout << "Function b is running\n";
//             std::this_thread::sleep_for(std::chrono::seconds(5));
//         }
//         << []()
//         {
//             std::this_thread::sleep_for(std::chrono::seconds(3));
//             std::cout << "Function c is running\n";
//         }
//         << []()
//         {
//             std::cout << "Function d is running\n";
//         };
//     };

//     go >> []()
//     {
//         std::cout << "Function e is running\n";
//     };

//     return 0;
// }

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

    // استفاده از Future و Promise
    template <typename Func>
    auto operator+(Func &&func)
    {
        return std::async(std::launch::async, std::forward<Func>(func));
    }

    // استفاده از Thread Pool
    template <typename Func>
    jthreadTaskClass &operator&(Func &&func)
    {
        auto future = thread_pool_.enqueue(std::forward<Func>(func));
        future.get(); // Wait for the task to complete
        return *this;
    }

private:
    std::vector<std::jthread> tasks_;
    ThreadPool thread_pool_{4}; // ایجاد یک استخر با 4 نخ
};

jthreadTaskClass go;

int main()
{
    go >> []() {
        go << []() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "Function a is running\n";
        }
        << []() {
            std::cout << "Function b is running\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        << []() {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "Function c is running\n";
        }
        << []() {
            std::cout << "Function d is running\n";
        };
    };

    go >> []() {
        std::cout << "Function e is running\n";
    };

    // استفاده از Future و Promise
    go + []() {
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Future task is running\n";
    };

    // استفاده از Thread Pool
    go & []() {
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Thread pool task is running\n";
    };

    return 0;
}
