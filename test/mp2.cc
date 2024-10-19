#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <thread> // برای استفاده از std::this_thread::sleep_for

class FireAndForget
{
public:
    // اجرای تسک با استفاده از OpenMP
    template <typename Func>
    FireAndForget &operator>>(Func &&func)
    {
#pragma omp task
        {
            func(); // اجرای تابع
        }
        return *this;
    }

    // انتظار برای تمام تسک‌ها
    void wait()
    {
#pragma omp taskwait
    }
};

// تابع برای تأخیر در اجرای تسک
void delayed_task(int id, int delay)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // تأخیر
    std::cout << "Task " << id << " executed after " << delay << " ms\n";
}

// تست FireAndForget با OpenMP
void fire_test()
{
    FireAndForget fire;

    // اضافه کردن تسک‌ها با تأخیرهای متفاوت
    fire >> [] { delayed_task(1, 1000); } // تسک 1 با تأخیر 1000 میلی‌ثانیه
        >> [] { delayed_task(2, 500); }   // تسک 2 با تأخیر 500 میلی‌ثانیه
        >> [] { delayed_task(3, 300); }   // تسک 3 با تأخیر 300 میلی‌ثانیه
        >> [] { delayed_task(4, 200); };  // تسک 4 با تأخیر 200 میلی‌ثانیه

    // صبر کردن برای اتمام تسک‌ها
    std::cout << "Waiting for all tasks to complete...\n";
    fire.wait();

    std::cout << "All tasks completed.\n";
}

int main()
{
    // تنظیم تعداد رشته‌های OpenMP
    omp_set_dynamic(0);     // غیرفعال کردن تنظیم دینامیک تعداد رشته‌ها
    omp_set_num_threads(4); // تنظیم تعداد رشته‌ها

    fire_test();
    return 0;
}
