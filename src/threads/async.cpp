// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class async {
public:
    template<typename Func>
    async(Func&& func) {
        future_ = std::async(std::launch::async, std::forward<Func>(func));
    }

    template<typename Func>
    async& operator=(Func&& val) {
        future_ = std::async(std::launch::async, std::forward<Func>(val));
        return *this;
    }

    auto get() {
        return future_.get();
    }

private:
    template<typename Func>
    using return_type = typename std::invoke_result<Func>::type;

    std::future<return_type<std::function<void()>>> future_;
};

// int main() {
//     async a = lm{ 
//         std::cout << "Hello from async!" << std::endl; 
//     };

//     a.get();

//     return 0;
// }
