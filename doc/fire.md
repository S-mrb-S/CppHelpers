# FireAndForget

The `FireAndForget` class provides an efficient mechanism for asynchronously executing tasks in C++. It supports two primary modes of execution:

- **Fire-and-Forget (`<<`)**: Executes tasks immediately without waiting for completion.
- **Fire-and-Wait (`>>`)**: Queues tasks for later execution, allowing them to be awaited.

## Features

- **Parallel Task Execution**: Run multiple tasks in parallel using threading.
- **Error Handling**: Catches and logs exceptions during task execution.
- **Flexible Execution**: Choose between immediate execution and future-based task management.

## Limitations

- **Fire-and-Forget (`<<`)**: Tasks may not complete if the program exits prematurely.
- **Fire-and-Wait (`>>`)**: You must explicitly wait for tasks to finish using the `waitForAllTasks()` method; otherwise, they might not complete.

## Usage

Here’s an example of how to use the `FireAndForget` class:

### Fire-and-Wait Execution (`>>`)

Tasks can be added to a queue for future execution. You can use the `waitForAllTasks()` method to ensure that all tasks complete before continuing.

```cpp
fire >> [] {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Task 1 executed\n";
};

fire >> [] {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task 2 executed\n";
};
```

### Fire-and-Forget Execution (`<<`)

You can execute tasks immediately without waiting for them to finish. This is useful for non-blocking operations.

```cpp
fire << [] {
    std::cout << "Immediate Task executed\n";
};
```

# Waiting for All Tasks

To ensure that all tasks finish, use the following method:
```
fire.waitForAllTasks();
```

# Complete Example

```
void fire_test() {
    fire >> [] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 1 executed\n";
    };

    fire >> [] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 2 executed\n";
    };

    fire << [] {
        std::cout << "Immediate Task executed\n";
    };

    std::cout << "Waiting for all tasks to complete...\n";
    fire.waitForAllTasks();
    std::cout << "All tasks completed.\n";
}
```