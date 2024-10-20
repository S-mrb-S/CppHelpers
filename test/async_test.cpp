// int main() {
//     // استفاده از ماکرو برای ایجاد یک لامبدا
//     async myAsync = hellofun(5);

//     async myAsync = lm {
//             std::cout << "Hi, Params: Some parameters" << std::endl;
//         };

//     async a = []() { std::cout << "Function a\n"; };
//     async b = []() { std::cout << "Function b\n"; };
//     async c = []() { std::cout << "Function c\n"; };

//     // زنجیره‌ای کردن انتساب‌ها
//     a = b = c; // این خط به ترتیب c به b و سپس b به a انتساب می‌دهد

//     return 0;
// }

// #include<iostream>
// using namespace std;

// // Anonymous Class : Class is not having any name
// typedef class
// {
//     // data member
//     int i;
// public:
//     void setData(int i)
//     {
//         // this pointer is used to differentiate
//         // between data member and formal argument.
//         this->i = i;
//     }
//     void print()
//     {
//         cout << "Value for i :" << this->i << endl;
//     }

// } myClass;      // using typedef give a proper name

// // Driver function
// int main()
// {
//     // multiple objects
//     myClass obj1, obj2;
//     obj1.setData(10);
//     obj1.print();

//     obj2.setData(20);
//     obj2.print();
//     return 0;
// }

// #include <iostream>
// using namespace std;

// class AnonymousClass
// {
//     int i;

// public:
//     // Function to set data
//     void setData(int i)
//     {
//         this->i = i;
//     }

//     // Friend function to overload << for printing
//     friend ostream &operator<<(ostream &out, const AnonymousClass &obj);

//     // Overload << operator to take an integer and set data
//     AnonymousClass &operator<<(int value)
//     {
//         this->setData(value);
//         return *this;
//     }
// } go;

// // Implementation of << operator for printing
// ostream &operator<<(ostream &out, const AnonymousClass &obj)
// {
//     out << "Value for i: " << obj.i;
//     return out;
// }

// // Driver function
// int main()
// {
//     // Using the new << operator to set data and print
//     go << 10;           // Set data using << operator
//     cout << go << endl; // Print the data

//     // You can chain this operation if needed:
//     go << 20 << 30;     // Set data multiple times
//     cout << go << endl; // Final print

//     return 0;
// }

// #include <iostream>
// #include <future>
// #include <chrono>
// #include <thread>
// #include <vector>

// class asyncTaskClass {
// public:
//     // سازنده پیش‌فرض
//     asyncTaskClass() = default;

//     // سازنده برای ایجاد یک شی asyncTaskClass از یک لامبدا
//     template<typename Func>
//     asyncTaskClass(Func&& func) {
//         future_ = std::async(std::launch::async, std::forward<Func>(func));
//     }

//     // عملگر هم‌ارزی برای اضافه کردن توابع لامبدا
//     template<typename Func>
//     asyncTaskClass& operator<<(Func&& func) {
//         // ایجاد یک شی جدید async و اضافه کردن آن به لیست
//         tasks_.emplace_back(std::async(std::launch::async, std::forward<Func>(func)));
//         return *this; // برگشت این شی برای زنجیره‌ای کردن
//     }

//     // تابع برای گرفتن نتایج
//     void get_all() {
//         for (auto& task : tasks_) {
//             task.get();
//         }
//     }

// private:
//     std::vector<std::future<void>> tasks_; // وکتور برای ذخیره وظایف
//     std::future<void> future_; // وظیفه جاری (که دیگر به آن نیاز نیست)
// };

// asyncTaskClass xgo;

// int main() {

//     // استفاده از عملگر << برای اضافه کردن توابع لامبدا
//     xgo << []() {
//         std::this_thread::sleep_for(std::chrono::seconds(2)); // شبیه‌سازی کار
//         std::cout << "Function a is running\n";
//     } << []() {
//         std::cout << "Function b is running\n";
//         std::this_thread::sleep_for(std::chrono::seconds(1)); // شبیه‌سازی کار
//     } << []() {
//         std::this_thread::sleep_for(std::chrono::seconds(3)); // شبیه‌سازی کار
//         std::cout << "Function c is running\n";
//     } << []() {
//         std::cout << "Function d is running\n";
//     };

//     xgo << []() {
//         std::this_thread::sleep_for(std::chrono::seconds(2)); // شبیه‌سازی کار
//         std::cout << "Function a is running\n";
//     };

//     // صبر کردن برای اتمام تمام وظایف
//     xgo.get_all();

//     return 0;
// }

// class asyncTaskClass {
// public:
//     template<typename Func>
//     asyncTaskClass& operator>>(Func&& func) {
//         std::async(std::launch::async, std::forward<Func>(func)).get();
//         return *this;
//     }

//     template<typename Func>
//     asyncTaskClass& operator<<(Func&& func) {
//         tasks_.emplace_back(std::async(std::launch::async, std::forward<Func>(func)));
//         return *this;
//     }

//     void get_all() {
//         for (auto& task : tasks_) {
//             task.get();
//         }
//     }

// private:
//     std::vector<std::future<void>> tasks_;
// };

// asyncTaskClass xgo;

// int main(){

//     xgo << []() {
//         std::this_thread::sleep_for(std::chrono::seconds(2));
//         std::cout << "Function a is running\n";
//     } << []() {
//         std::cout << "Function b is running\n";
//     } << []() {
//         std::this_thread::sleep_for(std::chrono::seconds(3));
//         std::cout << "Function c is running\n";
//     } << []() {
//         std::cout << "Function d is running\n";
//     };

//     xgo >> []() {
//         std::cout << "Function X is running\n";
//         exit(0);
//     };

//     xgo.get_all();

//     return 0;
// }

#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

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
        return *this << std::make_pair(std::forward<Func>(func), false); // پیش‌فرض async
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

asyncTaskClass xgo;

// int main()
// {
//     xgo << []()
//     {
//         std::this_thread::sleep_for(std::chrono::seconds(2));
//         std::cout << "Function a is running\n";
//     } // اجرا به صورت async به صورت پیش‌فرض
//         << std::make_pair([]()
//                           { std::cout << "Function b is running\n"; }, true) // اجرا به صورت async
//         << std::make_pair([]()
//                           {
//         std::this_thread::sleep_for(std::chrono::seconds(3));
//         std::cout << "Function c is running\n"; }, false) // اجرا به صورت deferred
//         << std::make_pair([]()
//                           { std::cout << "Function d is running\n"; }, false) >>
//         []()
//     {
//         std::cout << "Function X is running\n";
//         // _Exit(0);
//     };

//     xgo  >>
//         []()
//     {
//         std::cout << "Function X is running\n";
//         // _Exit(0);
//     };

//     xgo.get_all();

//     return 0;
// }

void xgo_test()
{

    // safe is >>
    xgo << []()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 1 executed\n";
    };

    xgo << []()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 2 executed\n";
    };

    // qq is <<
    // اجرای فوری یک تسک با استفاده از عملگر <<
    xgo << []()
    {
        // xgo xgo;
        xgo << []()
        {
            std::cout << "Immediate Task executed1\n";
            xgo << []()
            {
                std::cout << "Immediate Task executed2\n";
            };
        };
    };

    xgo << []()
    {
        std::cout << "Immediate Task executed3\n";
    };

    xgo << []()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Task 3 executed\n";
    };

    // صبر کردن برای اتمام تسک‌ها
    std::cout << "Waiting for all tasks to complete...\n";
    // xgo.get_all();

    // std::cout >> "All tasks completed.\n";

}


// Main function
int main() {
    for (size_t i = 0; i < 50000; i++) // کاهش تعداد تسک‌ها
    {
        // xgo << std::make_pair([]()
        //                    { 
        //                         xgo_test();
        //                     }, true);
        xgo >> []()
        {
            xgo_test();
        };
    }
    
    xgo.get_all();
    // fire.waitForAllTasks(); // صبر کردن برای اتمام تسک‌ها

    return 0;
}
