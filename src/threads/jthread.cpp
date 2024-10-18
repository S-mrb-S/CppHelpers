// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class jthreadTaskClass
{
public:
    // سازنده پیش‌فرض
    jthreadTaskClass() = default;

    // عملگر هم‌ارزی برای اضافه کردن توابع لامبدا
    template <typename Func>
    jthreadTaskClass &operator<<(Func &&func)
    {
        // ایجاد یک jthread و اضافه کردن آن به لیست
        tasks_.emplace_back(std::jthread([func = std::forward<Func>(func)](std::stop_token st)
                                         {
            // بررسی stop_token برای توقف امن
            if (st.stop_requested()) {
                return; // در صورت درخواست توقف، تابع را ترک می‌کنیم
            }
            func(); }));
        return *this; // برگشت این شی برای زنجیره‌ای کردن
    }

private:
    std::vector<std::jthread> tasks_; // وکتور برای ذخیره jthreadها
};

jthreadTaskClass go;