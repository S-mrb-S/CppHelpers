#include "include/hc.hpp"


fn hellofun(int hi){

}

run {
    let arr1 = 10;
    // let arr1 = 3.14;
    hellofun(5);

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

    end;
}
