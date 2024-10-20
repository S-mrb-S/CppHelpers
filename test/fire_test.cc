#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <stdexcept>

void heavyTask() {
    // شبیه‌سازی یک تسک سنگین
    for (volatile int i = 0; i < 1e7; ++i) {
        // یک عملیات بی‌هدف برای استفاده از CPU
    }
}

template<typename Func>
void runParallel(Func&& func) {
    std::jthread([func = std::forward<Func>(func)]()
                 {
        try {
            if (std::uncaught_exceptions() > 0) {
                throw std::runtime_error("Memory is being deallocated, stopping task.");
            }
            func();
        } catch (const std::exception& e) {
            std::cerr << "Exception in immediate task: " << e.what() << '\n';
        } catch (...) {
            std::cerr << "Unknown exception in immediate task.\n";
        } }).detach();
}

int main() {
    const int numTasks = 10; // تعداد تسک‌ها
    std::vector<std::jthread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    // اجرای تسک‌ها به صورت موازی
    for (int i = 0; i < numTasks; ++i) {
        runParallel(heavyTask);
    }

    // صبر برای اتمام همه تسک‌ها
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
    return 0;
}
