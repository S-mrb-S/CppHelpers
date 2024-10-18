// int main() {
//     // استفاده از ماکرو برای ایجاد یک لامبدا
//     async myAsync = hellofun(5);

//     async myAsync = lm {
//             std::cout << "Hi, Params: Some parameters" << std::endl;
//         };

//     async a = []() { std::cout << "Function a\n"; };
//     async b = []() { std::cout << "Function b\n"; };
//     async c = []() { std::cout << "Function c\n"; };

//     // زنجیره‌ای کردن انتساب‌ها
//     a = b = c; // این خط به ترتیب c به b و سپس b به a انتساب می‌دهد

//     return 0;
// }

// #include<iostream>
// using namespace std;

// // Anonymous Class : Class is not having any name
// typedef class
// {
//     // data member
//     int i;
// public:
//     void setData(int i)
//     {
//         // this pointer is used to differentiate
//         // between data member and formal argument.
//         this->i = i;
//     }
//     void print()
//     {
//         cout << "Value for i :" << this->i << endl;
//     }

// } myClass;      // using typedef give a proper name

// // Driver function
// int main()
// {
//     // multiple objects
//     myClass obj1, obj2;
//     obj1.setData(10);
//     obj1.print();

//     obj2.setData(20);
//     obj2.print();
//     return 0;
// }

// #include <iostream>
// using namespace std;

// class AnonymousClass
// {
//     int i;

// public:
//     // Function to set data
//     void setData(int i)
//     {
//         this->i = i;
//     }

//     // Friend function to overload << for printing
//     friend ostream &operator<<(ostream &out, const AnonymousClass &obj);

//     // Overload << operator to take an integer and set data
//     AnonymousClass &operator<<(int value)
//     {
//         this->setData(value);
//         return *this;
//     }
// } go;

// // Implementation of << operator for printing
// ostream &operator<<(ostream &out, const AnonymousClass &obj)
// {
//     out << "Value for i: " << obj.i;
//     return out;
// }

// // Driver function
// int main()
// {
//     // Using the new << operator to set data and print
//     go << 10;           // Set data using << operator
//     cout << go << endl; // Print the data

//     // You can chain this operation if needed:
//     go << 20 << 30;     // Set data multiple times
//     cout << go << endl; // Final print

//     return 0;
// }

#include <iostream>
#include <future>
#include <chrono>
#include <thread>

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
