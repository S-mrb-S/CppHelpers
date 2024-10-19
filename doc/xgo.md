# asyncTaskClass OR async OR xgo

The `asyncTaskClass` provides a flexible and efficient way to manage and execute asynchronous and deferred tasks in C++. It allows for both immediate and queued task execution with control over the concurrency level.

## Features

- **Immediate and Deferred Execution**: Tasks can be executed immediately or deferred for later execution.
- **Task Queue Management**: Queues tasks and manages them based on the specified number of threads.
- **Asynchronous Execution**: Supports both asynchronous and deferred task execution modes.
- **Thread Limiting**: Limits the number of concurrent threads using `max_threads`.

## Usage

Here’s an example of how to use the `asyncTaskClass`:

### Asynchronous Execution (Default)

Tasks are executed asynchronously by default when using the `<<` operator.

```cpp
xgo << []() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Function a is running\n";
};
```

# Deferred Execution

Deferred execution allows you to schedule tasks that will only execute when explicitly invoked.
```
xgo << std::make_pair([]() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Function c is running\n";
}, false);  // false means deferred execution
```

## Immediate Execution with (>>)

To execute a task immediately and wait for its completion, use the (>>) operator.
```
xgo >> []() {
    std::cout << "Function X is running\n";
};
```

# Complete Example
```
int main() {
    xgo << []() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Function a is running\n";
    } << std::make_pair([]() {
        std::cout << "Function b is running\n";
    }, true) << std::make_pair([]() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Function c is running\n";
    }, false) << std::make_pair([]() {
        std::cout << "Function d is running\n";
    }, false) >> []() {
        std::cout << "Function X is running\n";
    };

    xgo >> []() {
        std::cout << "Function X is running\n";
    };

    xgo.get_all();

    return 0;
}
```

# Conclusion

The asyncTaskClass offers a simple yet powerful way to manage concurrent tasks in C++ applications. It balances performance with control, allowing developers to easily manage asynchronous and deferred tasks while controlling thread usage.