# jthreadTaskClass OR jthread OR go

The `jthreadTaskClass` is a versatile task execution class that uses `std::jthread` for thread management, integrates with a thread pool, and supports tasks using `std::promise` and `std::future`. This class simplifies asynchronous execution and thread management, providing flexibility for immediate execution, task queuing, and deferred execution.

## Features

- **Immediate Task Execution**: Run tasks immediately with `std::jthread`.
- **Task Queuing**: Queue tasks to be executed one after another using `<<`.
- **Promise and Future**: Allows waiting for task results with `std::promise` and `std::future`.
- **Thread Pool**: Efficient execution of tasks using a thread pool with adjustable thread count.

## Usage

### Immediate Task Execution

You can execute tasks immediately using the `>>` operator.

```cpp
go >> []() {
    std::cout << "Function e is running\n";
};
```
OR
```cpp
go >> lm { // with macros
    std::cout << "Function e is running\n";
};
```

# Queuing Tasks

To queue multiple tasks for sequential execution, use the << operator.
```
go << []() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Function a is running\n";
} << []() {
    std::cout << "Function b is running\n";
};
```

# Promise and Future

To use a std::future and wait for a task's completion, you can use the + operator.
```
auto future = go + []() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Future task is running\n";
};
future.get(); // Wait for the future task to complete
```

## Thread Pool Execution

Tasks can be executed via a thread pool using the = operator. This leverages multiple threads to handle task execution more efficiently.
```
go = []() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread pool task is running\n";
};
```

# Complete Example
```
int main() {
    go >> []() {
        // add all to tasks
        go << []() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "Function a is running\n";
        } << []() {
            std::cout << "Function b is running\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        } << []() {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "Function c is running\n";
        } << []() {
            std::cout << "Function d is running\n";
        };
    };

    // this is faster
    go >> []() {
        std::cout << "Function e is running\n";
    };

    // Using Promise and Future
    auto future = go + []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Future task is running\n";
    };

    future.get(); // Wait for the future task to complete

    // Using Thread Pool
    go = []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Thread pool task is running\n";
    };

    return 0;
}
```