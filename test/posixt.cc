// posix threaad

#include <iostream>
#include <pthread.h>
#include <unistd.h>

// تابعی که توسط thread اجرا خواهد شد
void* print_message(void* arg) {
    const char* message = (const char*)arg;
    for (int i = 0; i < 5; ++i) {
        std::cout << message << " - Iteration " << i + 1 << std::endl;
        sleep(1); // متوقف کردن thread برای 1 ثانیه
    }
    return nullptr;
}

int main() {
    pthread_t thread1, thread2;

    // ایجاد دو thread که هر کدام تابع print_message را اجرا می‌کنند
    const char* message1 = "Thread 1";
    const char* message2 = "Thread 2";
    
    pthread_create(&thread1, nullptr, print_message, (void*)message1);
    pthread_create(&thread2, nullptr, print_message, (void*)message2);

    // منتظر می‌مانیم تا هر دو thread به اتمام برسند
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    std::cout << "Both threads have finished execution." << std::endl;
    return 0;
}
