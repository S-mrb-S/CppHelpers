// OpenMP

#include <iostream>
#include <omp.h>
#include <vector>
#include <functional>

class ParallelExecutor {
public:
    // Constructor to set the number of threads
    ParallelExecutor(int num_threads) {
        omp_set_num_threads(num_threads);
    }

    // Method to execute a lambda function in parallel
    template <typename Func>
    void execute(Func&& func, int num_iterations) {
        // Use OpenMP to parallelize the execution of the lambda function
        #pragma omp parallel for
        for (int i = 0; i < num_iterations; ++i) {
            func(i); // Call the lambda function with the current index
        }
    }
};

int main() {
    const int num_iterations = 10;

    // Create an instance of ParallelExecutor with a specified number of threads
    ParallelExecutor executor(4);

    // Execute a simple lambda function that prints the index
    executor.execute([](int i) {
        std::cout << "Thread " << omp_get_thread_num() << " is processing iteration " << i << std::endl;
    }, num_iterations);

    return 0;
}
