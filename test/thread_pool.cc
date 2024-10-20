#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    template<class Func>
    void enqueue(Func&& func);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

    void workerThread();
};

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });

            if (stop && tasks.empty()) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
    }
}

template<class Func>
void ThreadPool::enqueue(Func&& func) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::forward<Func>(func));
    }
    condition.notify_one();
}


void heavyTask() {
    // شبیه‌سازی یک تسک سنگین
    for (volatile int i = 0; i < 1e7; ++i) {
        // عملیات ساده برای مصرف CPU
    }
}

int main() {
    const int numTasks = 10; // تعداد تسک‌ها
    const int numThreads = 10; // تعداد نخ‌های موجود در Thread Pool

    ThreadPool pool(numThreads);

    auto start = std::chrono::high_resolution_clock::now();

    // اضافه کردن تسک‌ها به Thread Pool
    for (int i = 0; i < numTasks; ++i) {
        pool.enqueue(heavyTask);
    }

    // صبر برای اتمام همه تسک‌ها (در حالت عادی باید سیگنال دهی داشته باشید)
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}
