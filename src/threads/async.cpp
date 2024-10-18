// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class async {
public:
    async() = default;

    template<typename Func>
    async(Func&& func) {
        future_ = std::async(std::launch::async, std::forward<Func>(func));
    }

    template<typename Func>
    async& operator<<(Func&& func) {
        tasks_.emplace_back(std::async(std::launch::async, std::forward<Func>(func)));
        return *this;
    }

    void get_all() {
        for (auto& task : tasks_) {
            task.get();
        }
    }

private:
    std::vector<std::future<void>> tasks_;
    std::future<void> future_; // No needed
};

// int main() {
//     async task; // ایجاد یک شی async

//     // استفاده از عملگر << برای اضافه کردن توابع لامبدا
//     task << []() {
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

//     // صبر کردن برای اتمام تمام وظایف
//     task.get_all();

//     return 0;
// }
