#include <iostream>
#include <vector>
#include <memory>

#define PRINT(x) std::cout << x << std::endl

// کلاس مدیریت حافظه
class MemoryManager {
public:
    std::vector<std::unique_ptr<void, void(*)(void*)>> pointers;

    template<typename T>
    T* allocate(size_t size) {
        T* ptr = static_cast<T*>(malloc(size * sizeof(T)));
        if (ptr) {
            pointers.push_back({ ptr, free });
        } else {
            std::cerr << "Memory allocation failed" << std::endl;
        }
        return ptr;
    }

    void free_all() {
        pointers.clear();
    }

    ~MemoryManager() {
        free_all();
    }
};

// ایجاد شیء سراسری MemoryManager
MemoryManager globalManager;

// کلاس ManagedVar که حافظه را مدیریت می‌کند
template<typename T>
class ManagedVar {
public:
    T* data;

    // سازنده تبدیل که امکان مقداردهی اولیه با استفاده از '=' را فراهم می‌کند
    ManagedVar(T value) {
        data = globalManager.allocate<T>(1); // تخصیص حافظه برای یک مقدار
        *data = value;
    }

    // اپراتور دسترسی به مقدار
    T& operator*() {
        return *data;
    }

    // دسترسی به پوینتر
    T* operator->() {
        return data;
    }

    // اپراتور تخصیص برای مقداردهی بعد از ایجاد شیء
    ManagedVar& operator=(T value) {
        *data = value;
        return *this;
    }
};

int main() {
    // حالا به صورت ساده‌تر متغیرها تعریف می‌شوند
    ManagedVar<int> arr1 = 10;       // متغیر int با مقدار اولیه 10
    ManagedVar<double> arr2 = 3.14;  // متغیر double با مقدار اولیه 3.14

    // دسترسی به مقادیر متغیرها
    PRINT("arr1: " << *arr1);
    PRINT("arr2: " << *arr2);

    // تغییر مقادیر متغیرها
    *arr1 = 20;
    *arr2 = 6.28;

    PRINT("arr1 (after modification): " << *arr1);
    PRINT("arr2 (after modification): " << *arr2);

    // حافظه به صورت خودکار در پایان برنامه آزاد می‌شود
    return 0;
}
