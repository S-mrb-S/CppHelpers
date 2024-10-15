#include <iostream>
#include <vector>
#include <memory>

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

// یک کلاس برای مدیریت متغیرها و تخصیص خودکار حافظه
template<typename T>
class ManagedVar {
public:
    T* data;
    MemoryManager& manager;

    // سازنده که مقدار اولیه می‌گیرد و حافظه تخصیص می‌دهد
    ManagedVar(MemoryManager& m, T value) : manager(m) {
        data = manager.allocate<T>(1); // تخصیص حافظه برای یک مقدار
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
};

int main() {
    MemoryManager manager;

    // ایجاد متغیرهای مدیریت شده
    ManagedVar<int> arr1(manager, 10);   // متغیر int با مقدار اولیه 10
    ManagedVar<double> arr2(manager, 3.14); // متغیر double با مقدار اولیه 3.14

    // دسترسی به مقادیر متغیرها
    std::cout << "arr1: " << *arr1 << std::endl;
    std::cout << "arr2: " << *arr2 << std::endl;

    // تغییر مقادیر متغیرها
    *arr1 = 20;
    *arr2 = 6.28;

    std::cout << "arr1 (after modification): " << *arr1 << std::endl;
    std::cout << "arr2 (after modification): " << *arr2 << std::endl;

    // حافظه به صورت خودکار در پایان برنامه آزاد می‌شود
    return 0;
}
