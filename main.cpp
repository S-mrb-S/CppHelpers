#include <iostream>
#include <vector>
#include <memory>

#define echo(x) std::cout << x << std::endl

#define Main int main

class MemoryManager
{
public:
    std::vector<std::unique_ptr<void, void (*)(void *)>> pointers;

    template <typename T>
    T *allocate(size_t size)
    {
        T *ptr = new T[size];
        if (ptr)
        {
            pointers.push_back(std::unique_ptr<void, void (*)(void *)>(ptr, [](void *p)
                                                                       { delete[] static_cast<T *>(p); }));
        }
        else
        {
            std::cerr << "Memory allocation failed" << std::endl;
        }
        return ptr;
    }

    void free_all()
    {
        pointers.clear();
    }

    ~MemoryManager()
    {
        free_all();
    }
};

MemoryManager globalManager;

template <typename T>
class ManagedVar
{
public:
    T *data;

    ManagedVar(T value)
    {
        data = globalManager.allocate<T>(1);
        if (!data)
        {
            throw std::runtime_error("Memory allocation failed");
        }
        *data = value;
    }

    T &operator*()
    {
        return *data;
    }

    T *operator->()
    {
        return data;
    }

    ManagedVar &operator=(T value)
    {
        *data = value;
        return *this;
    }
};

Main()
{
    ManagedVar<int> arr1 = 10;
    ManagedVar<double> arr2 = 3.14;

    echo("arr1: " << *arr1);
    echo("arr2: " << *arr2);

    arr1 = 20;
    *arr2 = 6.28;

    echo("arr1 (after modification): " << *arr1);
    echo("arr2 (after modification): " << *arr2);

    return 0;
}
