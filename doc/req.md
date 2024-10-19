## Requirements

This class is implemented using the following C++ standard libraries:

- **\<thread>**: For managing concurrent execution via `std::jthread`.
- **\<future>**: For handling asynchronous operations and waiting for task completion using `std::future` and `std::promise`.
- **\<chrono>**: For task timing and delays.
- **\<exception>**: For exception handling in multithreaded environments.

### C++ Version

The `FireAndForget` class requires **C++20** or later because it relies on:

- **`std::jthread`**: A thread that automatically joins when it goes out of scope, introduced in C++20.
- **Structured binding and other modern syntax features** that are part of C++20.

Make sure to compile your project with C++20 support enabled (`-std=c++20` for GCC/Clang or `/std:c++20` for MSVC).
