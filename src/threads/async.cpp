// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class asyncTaskClass
{
public:
    template <typename Func>
    asyncTaskClass &operator>>(Func &&func)
    {
        std::async(std::launch::async, std::forward<Func>(func)).get();
        return *this;
    }

    template <typename Func>
    asyncTaskClass &operator<<(Func &&func)
    {
        tasks_.emplace_back(std::async(std::launch::async, std::forward<Func>(func)));
        return *this;
    }

    void wait()
    {
        for (auto &task : tasks_)
        {
            task.get();
        }
    }

private:
    std::vector<std::future<void>> tasks_;
};

asyncTaskClass xgo;

fn async_test()
{

    xgo << lm
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Function a is running\n";
    }
    << lm
    {
        std::cout << "Function b is running\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    << lm
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Function c is running\n";
    }
    << lm
    {
        std::cout << "Function d is running\n";
    };

    xgo >> lm
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Function a is running\n";
    };

    // xgo.wait();

    endf;
}