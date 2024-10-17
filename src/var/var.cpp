// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

template <typename T>
class var
{
public:
    std::shared_ptr<T> data;

    var(T value)
    {
        data = std::make_shared<T>(value);
    }

    T &operator*()
    {
        return *data;
    }

    T *operator->()
    {
        return data.get();
    }

    var &operator=(T value)
    {
        *data = value;
        return *this;
    }
};
