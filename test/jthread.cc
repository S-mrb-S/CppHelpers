#include <iostream>
#include <thread>
#include <chrono>
#ifdef _WIN32
    #include <windows.h> // برای ویندوز
#else
    #include <pthread.h> // برای لینوکس
#endif

void threadFunction() {
    std::cout << "Thread is running...\n";
}

int main() {
    std::jthread jt(threadFunction);

    // دسترسی به native handle
    auto nativeHandle = jt.native_handle();
    
    #ifdef _WIN32
        // تغییر اولویت thread در ویندوز
        SetThreadPriority(nativeHandle, THREAD_PRIORITY_HIGHEST);
        std::cout << "Thread priority set to highest in Windows.\n";
    #else
        // تغییر ویژگی‌های thread در POSIX (لینوکس/یونیکس)
        sched_param sch_params;
        sch_params.sched_priority = 20; // اولویت بالا
        pthread_setschedparam(nativeHandle, SCHED_FIFO, &sch_params);
        std::cout << "Thread priority set to highest in POSIX.\n";
    #endif

    jt.join();
    return 0;
}
