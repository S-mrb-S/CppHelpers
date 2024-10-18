#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <functional>

class go {
public:
    // سازنده پیش‌فرض
    go() = default;

    // عملگر هم‌ارزی برای اضافه کردن توابع لامبدا
    template<typename Func>
    go& operator<<(Func&& func) {
        // ایجاد یک شی جدید jthread و اضافه کردن آن به لیست
        tasks_.emplace_back(std::jthread([func = std::forward<Func>(func)](std::stop_token stoken) mutable {
            // بررسی وضعیت توقف و اجرا
            if (stoken.stop_requested()) return;
            func();
        }));
        return *this; // برگشت این شی برای زنجیره‌ای کردن
    }

private:
    std::vector<std::jthread> tasks_; // وکتور برای ذخیره وظایف
};

int main() {
    go task; // ایجاد یک شی go

    // استفاده از عملگر << برای اضافه کردن توابع لامبدا
    task << []() {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // شبیه‌سازی کار
        std::cout << "Function a is running\n";
    } << []() {
        std::cout << "Function b is running\n";
        std::this_thread::sleep_for(std::chrono::seconds(1)); // شبیه‌سازی کار
    } << []() {
        std::this_thread::sleep_for(std::chrono::seconds(3)); // شبیه‌سازی کار
        std::cout << "Function c is running\n";
    } << []() {
        std::cout << "Function d is running\n";
    };

    return 0;
}
