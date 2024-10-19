#include <iostream>
#include <omp.h>
#include <chrono>
#include <thread>

void heavy_computation(int id)
{
    // شبیه‌سازی یک محاسبه سنگین
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // شبیه‌سازی تأخیر
    std::cout << "Heavy computation finished by thread " << id << "\n";
}


/*
با استفاده از parallel
و تنظیم تعداد رشته ها
فانکشن داخل task رو به اندازه تعداد رشته های تنظیم شده
اجرا میکند

یعنی هر رشته یکبار فانکشن را اجرا میکند به صورت موازی
 */
int main()
{
    // تنظیم تعداد رشته‌های OpenMP
    omp_set_dynamic(0);     // غیرفعال کردن تنظیم دینامیک تعداد رشته‌ها
    omp_set_num_threads(4000); // تنظیم تعداد رشته‌ها

#pragma omp parallel // ایجاد یک منطقه موازی
    {
        int thread_id = omp_get_thread_num(); // دریافت شناسه رشته

        // هر رشته تسک‌های سنگین را ایجاد می‌کند
#pragma omp task
        {
            heavy_computation(thread_id); // اجرای محاسبه سنگین
        }
    }

    // منتظر ماندن برای اتمام تمام تسک‌ها
#pragma omp taskwait // انتظار برای اتمام همه تسک‌ها

    std::cout << "All heavy computations are done.\n";
    return 0;
}
