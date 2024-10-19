// #include <iostream>
// #include <cilk/cilk.h>

// int main() {
//     const int N = 100000;
//     int sum = 0;

//     // Compute the sum of integers from 1 to N in parallel using Cilk Plus
//     cilk_for (int i = 1; i <= N; ++i) {
//         sum += i;
//     }

//     std::cout << "The sum of integers from 1 to " << N << " is " << sum << std::endl;

//     return 0;
// }
