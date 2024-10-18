// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

#include <iostream>
#include <thread>
#include <future>
#include <functional>

class go {
public:
    template<typename Func>
    go(Func&& func) {
        // Start the jthread and store the result in a promise.
        promise_ = std::make_shared<std::promise<void>>();
        jthread_ = std::jthread([func = std::forward<Func>(func), p = promise_] {
            func();
            p->set_value();  // Notify completion
        });
    }

    template<typename Func>
    go& operator=(Func&& func) {
        // Similar to constructor, start a new jthread with the passed function.
        if (jthread_.joinable()) {
            jthread_.join();
        }
        promise_ = std::make_shared<std::promise<void>>();
        jthread_ = std::jthread([func = std::forward<Func>(func), p = promise_] {
            func();
            p->set_value();  // Notify completion
        });
        return *this;
    }

    void get() {
        // Wait for the thread to complete.
        promise_->get_future().get();
        if (jthread_.joinable()) {
            jthread_.join();  // Ensure thread is joined.
        }
    }

private:
    std::jthread jthread_;
    std::shared_ptr<std::promise<void>> promise_;
};

void hi(){
        std::cout << "Hello from go!" << std::endl;
}
int main() {
    go a = hi;

    a.get();

    return 0;
}
