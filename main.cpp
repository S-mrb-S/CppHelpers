#include "include/hc.hpp"




class async {
public:
    template<typename Func>
    async(Func&& val) {
        std::async(std::launch::async, std::forward<Func>(val)).get();
    }

    template<typename Func>
    async& operator=(Func&& val) {
        std::async(std::launch::async, std::forward<Func>(val)).get();
    }
};



// fnc(public, MyClassName)
//     void display() {
//         std::cout << "Hello from class!" << std::endl;
//     }
// };

// fn hellofun(int hi){

// }

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





run {
    let arr1 = 10;
    // let arr1 = 3.14;
    // hellofun(5);

    let myLet(20);
    std::cout << myLet << std::endl;  // خروجی: 20 (int)

    // let myLet("Hello");
    // myLet << " World";  // اضافه کردن " World" به رشته
    // std::cout << myLet << std::endl;  // خروجی: Hello World

    // myLet >> "New Value";  // جایگزینی مقدار با "New Value"
    // std::cout << myLet << std::endl;  // خروجی: New Value

    // echo("arr1: " << *arr1);
    // echo("arr2: " << *arr2);

    // *arr1 = 20;
    // *arr2 = 6.28;

    // echo("arr1 (after modification): " << *arr1);
    // echo("arr2 (after modification): " << *arr2);

    run_end;
}
