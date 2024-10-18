// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class let
{
public:
    any data;

    template <typename T>
    let(T value)
    {
        data = value;
    }

    let &operator<<(const string &value)
    {
        if (data.type() == typeid(string))
        {
            any_cast<string>(data) += value;
        }
        else
        {
            echo("Error: Cannot use << with non-string types!");
        }
        return *this;
    }

    template <typename T>
    let &operator>>(T value)
    {
        data = value;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const let &obj)
    {

        if (obj.data.type() == typeid(int))
        {
            os << any_cast<int>(obj.data);
        }
        else if (obj.data.type() == typeid(string))
        {
            os << any_cast<string>(obj.data);
        }
        else
        {
            os << "Unsupported type";
        }

        return os;
    }
};
