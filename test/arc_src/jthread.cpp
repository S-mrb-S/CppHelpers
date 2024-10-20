// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class jthreadTaskClass
{
public:
    template <typename Func>
    jthreadTaskClass &operator>>(Func &&func)
    {
        std::jthread([func = std::forward<Func>(func)]func());
        return *this;
    }

    template <typename Func>
    jthreadTaskClass &operator<<(Func &&func)
    {
        std::jthread([func = std::forward<Func>(func)](std::stop_token st)
                                         {
            if (st.stop_requested()) {
                return;
            }
            func(); });
        return *this;
    }
};

jthreadTaskClass go;

fn jthread_test()
{
    go << lm
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

    go << lm
    {
        std::cout << "Function e is running\n";
    };
}