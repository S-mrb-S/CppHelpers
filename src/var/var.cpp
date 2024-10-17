
// From let.cpp, To echo.cpp

#include "../let/let.cpp"

template <typename T>
fnc var
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
