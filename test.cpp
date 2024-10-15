#include <iostream>
#include <string>

#define var auto

int main() {
    var myVar = std::string("Hello World"); // تعریف یک متغیر رشته‌ای
    var myInt = 5; // تعریف یک متغیر صحیح
    // var cars[4] = {"Volvo", "BMW", "Ford", "Mazda"};

    int myNum[3] = {10, 20, 30};
    auto myNum = new int[3]{10, 20, 30};

    // نمایش مقادیر اولیه
    std::cout << "myVar: " << myVar << ", myInt: " << myInt << std::endl;

    // تغییر مقادیر
    myInt = 8; // تغییر مقدار myInt

    // نمایش مقادیر جدید
    std::cout << "myVar: " << myVar << ", myInt: " << myInt << std::endl;

    return 0;
}
