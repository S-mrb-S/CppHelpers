// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class async {
public:
    // سازنده پیش‌فرض
    async() = default;

    // سازنده برای ایجاد یک شی async از یک لامبدا
    template<typename Func>
    async(Func&& func) {
        future_ = std::async(std::launch::async, std::forward<Func>(func));
    }

    // عملگر هم‌ارزی برای اضافه کردن توابع لامبدا
    template<typename Func>
    async& operator<<(Func&& func) {
        // اجرای لامبدا و ذخیره نتیجه
        future_ = std::async(std::launch::async, std::forward<Func>(func));
        return *this; // برگشت این شی برای زنجیره‌ای کردن
    }

    // تابع برای گرفتن نتیجه
    auto get() {
        return future_.get();
    }

private:
    std::future<void> future_;
};

int main() {
    async task; // ایجاد یک شی async با سازنده پیش‌فرض

    // استفاده از عملگر << برای اضافه کردن توابع لامبدا
    task << []() {
        std::cout << "Function a is running\n";
        std::this_thread::sleep_for(std::chrono::seconds(2)); // شبیه‌سازی کار
    } << []() {
        std::cout << "Function b is running\n";
        std::this_thread::sleep_for(std::chrono::seconds(1)); // شبیه‌سازی کار
    } << []() {
        std::cout << "Function c is running\n";
        std::this_thread::sleep_for(std::chrono::seconds(3)); // شبیه‌سازی کار
    } << []() {
        std::cout << "Function d is running\n";
        std::this_thread::sleep_for(std::chrono::seconds(2)); // شبیه‌سازی کار
    };

    // صبر کردن برای اتمام تابع آخر
    task.get();

    return 0;
}
