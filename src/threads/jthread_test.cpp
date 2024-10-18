// #include <iostream>
// #include <chrono>
// #include <thread>
// #include <vector>
// #include <functional>
// #include <any>

// class cthread {
// public:
//     // سازنده پیش‌فرض
//     cthread() = default;

//     // عملگر هم‌ارزی برای اضافه کردن توابع لامبدا
//     template<typename Func>
//     cthread& operator<<(Func&& func) {
//         // ایجاد یک شی جدید jthread و اضافه کردن آن به لیست
//         tasks_.emplace_back(std::jthread([func = std::forward<Func>(func)](std::stop_token stoken) mutable {
//             // بررسی وضعیت توقف و اجرا
//             if (stoken.stop_requested()) return;
//             func();
//         }));
//         return *this; // برگشت این شی برای زنجیره‌ای کردن
//     }

// private:
//     std::vector<std::jthread> tasks_; // وکتور برای ذخیره وظایف
// };

// #define go cthread() <<

// std::any myFun(){
//     std::this_thread::sleep_for(std::chrono::seconds(2)); // شبیه‌سازی کار
//     std::cout << "Function a is running\n";

//     return nullptr;
// }

// int main() {
//     // cthread task; // ایجاد یک شی go

//     // استفاده از عملگر << برای اضافه کردن توابع لامبدا
//     go []() {
//         std::this_thread::sleep_for(std::chrono::seconds(2)); // شبیه‌سازی کار
//         std::cout << "Function a is running\n";
//     };

//     return 0;
// }

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

class jthreadTaskClass
{
public:
    template <typename Func>
    jthreadTaskClass &operator>>(Func &&func)
    {
        std::jthread j_thread(std::forward<Func>(func));
        return *this;
    }

    template <typename Func>
    jthreadTaskClass &operator<<(Func &&func)
    {
        tasks_.emplace_back(std::jthread([func = std::forward<Func>(func)](std::stop_token st)
                                         {
            if (st.stop_requested()) {
                return;
            }
            func(); }));
        return *this;
    }

private:
    std::vector<std::jthread> tasks_;
};

jthreadTaskClass go;

int main()
{
    go >> []() {
        go << []()
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "Function a is running\n";
        }
        << []()
        {
            std::cout << "Function b is running\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        << []()
        {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "Function c is running\n";
        }
        << []()
        {
            std::cout << "Function d is running\n";
        };
    };

    go >> []()
    {
        std::cout << "Function e is running\n";
    };

    return 0;
}