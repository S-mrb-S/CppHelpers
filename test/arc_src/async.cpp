// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class asyncTaskClass
{
public:
    asyncTaskClass(size_t max_threads = std::thread::hardware_concurrency())
        : stop_flag(false), max_threads(max_threads), active_threads(0) {}

    // اجرای فوری یک تسک و انتظار برای پایان آن
    template <typename Func>
    asyncTaskClass &operator>>(Func &&func)
    {
        std::async(std::launch::async, std::forward<Func>(func)).get();
        return *this;
    }

    template <typename Func>
    asyncTaskClass &operator<<(std::pair<Func, bool> task_info)
    {
        auto &func = task_info.first;
        bool async_execution = task_info.second;

        if (async_execution)
        {
            enqueue_task([this, func]()
                         {
                if (!stop_flag) {
                    func();
                } });
        }
        else
        {
            tasks_.emplace_back(std::async(std::launch::deferred, [this, func]()
                                           {
                if (!stop_flag) {
                    func();
                } }));
        }

        return *this;
    }

    // افزودن تسک به صورت پیش‌فرض async
    template <typename Func>
    asyncTaskClass &operator<<(Func &&func)
    {
        return *this << std::make_pair(std::forward<Func>(func), true); // پیش‌فرض async
    }

    // توقف تمام تسک‌های در صف
    void stop()
    {
        stop_flag = true;
    }

    // اجرا و صبر برای تمام تسک‌ها
    void get_all()
    {
        for (auto &task : tasks_)
        {
            task.get();
        }
        process_queued_tasks();
    }

private:
    std::vector<std::future<void>> tasks_;
    std::queue<std::function<void()>> task_queue;
    std::atomic<bool> stop_flag;
    std::mutex queue_mutex;
    std::condition_variable task_cv;
    size_t max_threads;
    std::atomic<size_t> active_threads;

    // افزودن تسک به صف و اجرای آن بر اساس محدودیت تعداد threadها
    void enqueue_task(std::function<void()> task)
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        task_queue.push(task);
        task_cv.notify_one();

        std::thread([this]()
                    {
            std::function<void()> local_task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                task_cv.wait(lock, [this]() { return !task_queue.empty() || stop_flag; });

                if (stop_flag) return;
                local_task = task_queue.front();
                task_queue.pop();
            }

            active_threads++;
            local_task();
            active_threads--; })
            .detach();
    }

    // پردازش تمام تسک‌های در صف
    void process_queued_tasks()
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        while (!task_queue.empty())
        {
            task_cv.wait(lock, [this]()
                         { return active_threads < max_threads; });

            auto task = task_queue.front();
            task_queue.pop();

            active_threads++;
            std::thread([task, this]()
                        {
                task();
                active_threads--;
                task_cv.notify_one(); })
                .detach();
        }
    }
};

asyncTaskClass xgo; // تسک ها را در لیست به صورت صف و با محدودیت منابع سیستم اجرا میکند.. و قابلیت صبر کردن (deferred) را دارد و در نهایت تسک ها را به صورت امن اجرا و تا پایان نیافتن انها به صورت خودکار برنامه را باز نگه میدارد.

void async_test()
{
    xgo << []()
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Function a is running\n";
    } // اجرا به صورت async به صورت پیش‌فرض
        << std::make_pair([]()
                          { std::cout << "Function b is running\n"; }, true) // اجرا به صورت async
        << std::make_pair([]()
                          {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Function c is running\n"; }, false) // اجرا به صورت deferred
        << std::make_pair([]()
                          { std::cout << "Function d is running\n"; }, false) >>
        []()
    {
        std::cout << "Function X is running\n";
        // _Exit(0);
    };

    xgo  >>
        []()
    {
        std::cout << "Function X is running\n";
        // _Exit(0);
    };

    xgo.get_all();

}
