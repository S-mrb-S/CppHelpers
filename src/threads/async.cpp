// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class async {
public:
    template<typename Func>
    async(Func&& val) {
        std::async(std::launch::async, std::forward<Func>(val)).get();
    }

    template<typename Func>
    async& operator=(Func&& val) {
        std::async(std::launch::async, std::forward<Func>(val)).get();
    }
};