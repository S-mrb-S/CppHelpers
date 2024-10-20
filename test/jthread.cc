#include <iostream>
#include <thread>
#include <vector>

// تابعی که باید در نخ اجرا شود
void thread_task(int id) {
    std::cout << "Task " << id << " is running in thread." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // شبیه‌سازی کار
    std::cout << "Task " << id << " is completed." << std::endl;
}

// تابعی برای تنظیمات خاص سیستم‌عامل
void configure_thread(std::jthread& t) {
#if defined(_WIN32) || defined(_WIN64)
    // در ویندوز، می‌توانیم از SetThreadPriority استفاده کنیم
    HANDLE native_handle = t.native_handle();
    SetThreadPriority(native_handle, THREAD_PRIORITY_NORMAL); // استفاده از اولویت عادی
    std::cout << "Configured thread priority to NORMAL on Windows." << std::endl;
#elif defined(__linux__)
    // در لینوکس، می‌توانیم از pthread_setschedparam استفاده کنیم
    pthread_t native_handle = t.native_handle();
    struct sched_param param;
    param.sched_priority = 0; // اولویت عادی

    // تلاش برای تنظیم اولویت
    if (pthread_setschedparam(native_handle, SCHED_OTHER, &param) != 0) {
        std::cerr << "Failed to set thread priority on Linux. Using default priority." << std::endl;
    } else {
        std::cout << "Configured thread priority to SCHED_OTHER on Linux." << std::endl;
    }
#endif
}

int main() {
    const int num_tasks = 5; // تعداد تسک‌ها
    std::vector<std::jthread> threads;

    // ایجاد و راه‌اندازی نخ‌ها
    for (int i = 0; i < num_tasks; ++i) {
        threads.emplace_back(thread_task, i + 1);
        configure_thread(threads.back()); // پیکربندی نخ
    }

    // در اینجا می‌توانیم کارهای دیگری انجام دهیم
    std::cout << "All tasks are started." << std::endl;

    // وقتی نخ‌ها از محدوده خارج می‌شوند، به طور خودکار متوقف می‌شوند
    return 0;
}
