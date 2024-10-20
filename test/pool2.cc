#include <iostream>
#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

// کلاس ThreadPool
class MyClassThreadPool {
public:
    MyClassThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task(); // اجرای تابع
                }
            });
        }
    }

    ~MyClassThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &thread : threads) {
            thread.join();
        }
    }

    // عملگر >> برای دریافت تابع
    MyClassThreadPool& operator>>(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::move(task));
        }
        condition.notify_one(); // notify یک thread برای اجرای تابع
        return *this; // امکان chain کردن
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

int main() {
    MyClassThreadPool pool(4); // ایجاد Thread Pool با ۴ Thread

    // استفاده از Thread Pool و اجرای توابع lambda
    pool >> []() {
        std::cout << "Task 1 is running in thread " << std::this_thread::get_id() << std::endl;
    };

    pool >> []() {
        std::cout << "Task 2 is running in thread " << std::this_thread::get_id() << std::endl;
    } >> []() {
        std::cout << "Task 3 is running in thread " << std::this_thread::get_id() << std::endl;
    };

    // دادن زمانی برای اجرای thread ها قبل از اتمام برنامه
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
