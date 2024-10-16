#include <iostream>
#include <string>
#include <any>
#include <typeinfo>
#include <typeindex>

class let {
public:
    std::any data;
    std::type_index type;  // برای ذخیره نوع فعلی

    // سازنده برای مقداردهی به هر نوع
    template <typename T>
    let(T value) {
        data = value;
        type = std::type_index(typeid(T));
    }

    // عملگر = برای مقداردهی و تعیین نوع
    template <typename T>
    let &operator=(T value) {
        data = value;
        type = std::type_index(typeid(T));
        return *this;
    }

    // عملگر >> برای جایگزینی مقدار
    template <typename T>
    let &operator>>(T value) {
        // تبدیل به نوع اصلی
        if (type == std::type_index(typeid(int))) {
            data = std::any_cast<int>(value);  // تبدیل به int
        } else if (type == std::type_index(typeid(std::string))) {
            data = std::any_cast<std::string>(value);  // تبدیل به string
        }
        return *this;
    }

    // عملگر << برای ترکیب و اضافه کردن به مقدار
    let &operator<<(const std::string &value) {
        if (type == std::type_index(typeid(std::string))) {
            std::any_cast<std::string>(data) += value;  // اضافه کردن به رشته
        } else {
            std::cerr << "Error: Cannot use << with non-string types!" << std::endl;
        }
        return *this;
    }

    // عملگر << برای ترکیب و اضافه کردن به مقدار
    let &operator<<(int value) {
        if (type == std::type_index(typeid(std::string))) {
            std::any_cast<std::string>(data) += std::to_string(value);  // اضافه کردن به رشته
        } else {
            std::cerr << "Error: Cannot use << with non-string types!" << std::endl;
        }
        return *this;
    }

    // تابع برای بدست آوردن نوع
    std::string getType() const {
        return type.name();  // برگرداندن نام نوع
    }

    // عملگر برای نمایش مقدار
    friend std::ostream &operator<<(std::ostream &os, const let &obj) {
        if (obj.type == std::type_index(typeid(int))) {
            os << std::any_cast<int>(obj.data);
        } else if (obj.type == std::type_index(typeid(std::string))) {
            os << std::any_cast<std::string>(obj.data);
        } else {
            os << "Unsupported type";  // در صورت عدم پشتیبانی
        }
        return os;
    }
};

int main() {
    let arr1 = 10;  // مقداردهی اولیه
    std::cout << arr1 << " (Type: " << arr1.getType() << ")\n";  // خروجی: 10 (int)

    arr1 >> "Hello";  // جایگزینی مقدار
    std::cout << arr1 << " (Type: " << arr1.getType() << ")\n";  // خروجی: Hello (string)

    let arr2 = "Hi";  // مقداردهی به string
    std::cout << arr2 << " (Type: " << arr2.getType() << ")\n";  // خروجی: Hi (string)

    arr2 << 10;  // ترکیب
    std::cout << arr2 << " (Type: " << arr2.getType() << ")\n";  // خروجی: Hi10 (string)

    return 0;
}
