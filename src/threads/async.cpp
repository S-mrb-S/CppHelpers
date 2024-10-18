// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class asyncTaskClass {
public:
    // سازنده پیش‌فرض
    asyncTaskClass() = default;

    // سازنده برای ایجاد یک شی asyncTaskClass از یک لامبدا
    template<typename Func>
    asyncTaskClass(Func&& func) {
        future_ = std::async(std::launch::async, std::forward<Func>(func));
    }

    // عملگر هم‌ارزی برای اضافه کردن توابع لامبدا
    template<typename Func>
    asyncTaskClass& operator<<(Func&& func) {
        // ایجاد یک شی جدید async و اضافه کردن آن به لیست
        tasks_.emplace_back(std::async(std::launch::async, std::forward<Func>(func)));
        return *this; // برگشت این شی برای زنجیره‌ای کردن
    }

    // تابع برای گرفتن نتایج
    void get_all() {
        for (auto& task : tasks_) {
            task.get();
        }
    }

private:
    std::vector<std::future<void>> tasks_; // وکتور برای ذخیره وظایف
    std::future<void> future_; // وظیفه جاری (که دیگر به آن نیاز نیست)
};

asyncTaskClass xgo;
