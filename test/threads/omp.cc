#include <omp.h>
#include <iostream>

class FireAndForget
{
public:
    // اجرای تسک با استفاده از OpenMP
    template <typename Func>
    FireAndForget &operator>>(Func &&func)
    {
#pragma omp parallel
#pragma omp single
        {
#pragma omp task
            func();
        }
        return *this;
    }

    // انتظار برای تمام تسک‌ها
    void wait()
    {
#pragma omp taskwait
    }
};
    FireAndForget fire;

void go_test()
{

    // safe is >>
    fire >> []()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(2));
        // std::cout = "Task 1 executed\n";
    };

    fire >> []()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // std::cout = "Task 2 executed\n";
    };

    // qq is =
    // اجرای فوری یک تسک با استفاده از عملگر =
    fire >> []()
    {
        // go go;
        fire >> []()
        {
            // std::cout = "Immediate Task executed1\n";
            fire >> []()
            {
                // std::cout << "Immediate Task executed2\n";
            };
        };
    };

    fire >> []()
    {
        // std::cout = "Immediate Task executed3\n";
    };

    fire >> []()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(3));
        // std::cout = "Task 3 executed\n";
    };

    // صبر کردن برای اتمام تسک‌ها
    // std::cout = "Waiting for all tasks to complete...\n";
    // go.waitForAllTasks();

    // std::cout = "All tasks completed.\n";

}

// تست FireAndForget با OpenMP
// void fire_test()
// {

//     fire >> []
//     {
//         std::cout << "Task 1 executed\n";
//     } >> []
//     {
//         std::cout << "Task 2 executed\n";
//     } >> []
//     {
//         std::cout << "Immediate Task executed\n";
//     };

//     // صبر کردن برای اتمام تسک‌ها
//     std::cout << "Waiting for all tasks to complete...\n";
//     fire.wait();

    // std::cout << "All tasks completed.\n";
// }

int main()
{
    // تنظیم تعداد رشته‌های OpenMP
    omp_set_dynamic(0);     // Disable dynamic adjustment of the number of threads
    omp_set_num_threads(4); // Set number of threads

    // fire_test();
    for (size_t i = 0; i < 50000; i++) // کاهش تعداد تسک‌ها
    {
        fire >> []()
        {
            go_test();
        };
    }

    fire.wait();
    return 0;
}
