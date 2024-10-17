
// From topc.hpp, To let.cpp

#include "../core/topc.hpp"

#define fun std::any
#define to std::any_cast
// functions for all types
// fun myFuncInCpp() {
//     if (false) {
//         return 5; // int
//     }
//     if (false) {
//         return false; // bool
//     }
//     return {}; // مشابه void
// }

// int main() {
//     auto result = myFuncInCpp();

//     if (result.has_value()) {
//         try {
//             std::cout << "Returned int: " << to<int>(result) << std::endl;
//         } catch (const std::bad_any_cast&) {
//             // Try to cast to bool
//             try {
//                 std::cout << "Returned bool: " << to<bool>(result) << std::endl;
//             } catch (const std::bad_any_cast&) {
//                 std::cout << "No valid value returned." << std::endl;
//             }
//         }
//     } else {
//         std::cout << "No value returned." << std::endl;
//     }

//     return 0;
// }

// #define main int main // Usage for: fun
#define func class // The class, exmaple: func MyClass
#define fn int main() // The main function, make it for test or run current file, example: fun { echo("Hello World") }

#define str std::string
// #define i int
// #define dub double
// #define fn void
