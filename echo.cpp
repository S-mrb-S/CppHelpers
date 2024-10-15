#include <iostream>
#include <string>
#include <sstream>

class Echo {
public:
    std::ostringstream stream;

    // متد برای اضافه کردن داده‌ها به استریم
    template<typename T>
    Echo operator+(const T& value) const {
        Echo newEcho = *this;  // یک کپی از خود ایجاد کنید
        newEcho.stream << value;  // اضافه کردن مقدار به استریم
        return newEcho;  // بازگرداندن شی جدید
    }

    // متد برای چاپ داده‌ها
    void print() const {
        std::cout << stream.str() << std::endl;
    }
};

// ماکرویی برای ساده‌سازی استفاده از کلاس Echo
#define echo(value) (Echo().operator+(value)).print()

int main() {
    int arr1 = 10;
    double arr2 = 3.14;

    // استفاده از echo با اپراتور +
    echo("arr1 (after modification): " + std::to_string(arr1));
    echo("arr1 (after modification): " + std::to_string(arr1) + " and arr2: " + std::to_string(arr2));

    return 0;
}
