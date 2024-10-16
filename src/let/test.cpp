// #include <iostream>
// #include <string>
// #include <any>
// #include <typeinfo>
// #include <typeindex>

// class let {
// public:
//     std::any data;
//     std::type_index type;  // برای ذخیره نوع فعلی
//     bool isInitialized = false; // برای پیگیری اینکه آیا متغیر مقداردهی اولیه شده است یا خیر

//     // سازنده برای مقداردهی به هر نوع
//     template <typename T>
//     let(T value) {
//         data = value;
//         type = std::type_index(typeid(T));
//         isInitialized = true;
//     }

//     // سازنده برای پشتیبانی از template بدون مقدار
//     let() : type(std::type_index(typeid(std::any))) {}

//     // عملگر = برای مقداردهی و تعیین نوع
//     // فقط در زمان تعریف اولیه کار می‌کند
//     template <typename T>
//     let &operator=(T value) {
//         std::cerr << "Error: Use 'let<T> variable = value;' for initialization!" << std::endl;
//         return *this;
//     }

//     // عملگر >> برای جایگزینی مقدار
//     template <typename T>
//     let &operator>>(T value) {
//         if (isInitialized) {
//             // تبدیل به نوع اصلی
//             if (type == std::type_index(typeid(int))) {
//                 data = std::any_cast<int>(value);  // تبدیل به int
//             } else if (type == std::type_index(typeid(std::string))) {
//                 data = std::any_cast<std::string>(value);  // تبدیل به string
//             } else {
//                 std::cerr << "Error: Unsupported type for replacement!" << std::endl;
//             }
//         } else {
//             std::cerr << "Error: Variable not initialized!" << std::endl;
//         }
//         return *this;
//     }

//     // عملگر << برای ترکیب و اضافه کردن به مقدار
//     let &operator<<(const std::string &value) {
//         if (type == std::type_index(typeid(std::string))) {
//             std::any_cast<std::string>(data) += value;  // اضافه کردن به رشته
//         } else {
//             std::cerr << "Error: Cannot use << with non-string types!" << std::endl;
//         }
//         return *this;
//     }

//     // عملگر << برای ترکیب و اضافه کردن به مقدار
//     let &operator<<(int value) {
//         if (type == std::type_index(typeid(std::string))) {
//             std::any_cast<std::string>(data) += std::to_string(value);  // اضافه کردن به رشته
//         } else {
//             std::cerr << "Error: Cannot use << with non-string types!" << std::endl;
//         }
//         return *this;
//     }

//     // تابع برای بدست آوردن نوع
//     std::string getType() const {
//         return type.name();  // برگرداندن نام نوع
//     }

//     // عملگر برای نمایش مقدار
//     friend std::ostream &operator<<(std::ostream &os, const let &obj) {
//         if (obj.type == std::type_index(typeid(int))) {
//             os << std::any_cast<int>(obj.data);
//         } else if (obj.type == std::type_index(typeid(std::string))) {
//             os << std::any_cast<std::string>(obj.data);
//         } else {
//             os << "Unsupported type";  // در صورت عدم پشتیبانی
//         }
//         return os;
//     }
// };

// int main() {
//     // استفاده از let با تعریف نوع
//     let<std::string> arr1 = "Hi";  // تعریفی از نوع std::string
//     arr1 = "Hello";  // پیغام خطا خواهد داد
//     std::cout << arr1 << " (Type: " << arr1.getType() << ")\n";  // خروجی: (هیچ مقداری ندارد)

//     arr1 = std::string("Hello"); // کاربر باید اینجا از let استفاده کند
//     std::cout << arr1 << " (Type: " << arr1.getType() << ")\n";  // خروجی: Hello (string)

//     arr1 << " World";  // ترکیب
//     std::cout << arr1 << " (Type: " << arr1.getType() << ")\n";  // خروجی: Hello World (string)

//     // حالا یک مثال با int
//     let<int> arr2;  // تعریفی از نوع int
//     arr2 = 10;  // پیغام خطا خواهد داد
//     std::cout << arr2 << " (Type: " << arr2.getType() << ")\n";  // خروجی: (هیچ مقداری ندارد)

//     arr2 = 10; // کاربر باید اینجا از let استفاده کند
//     std::cout << arr2 << " (Type: " << arr2.getType() << ")\n";  // خروجی: 10 (int)

//     arr2 >> 20;  // جایگزینی
//     std::cout << arr2 << " (Type: " << arr2.getType() << ")\n";  // خروجی: 20 (int)

//     return 0;
// }
