
// From macros.cpp, To var.cpp

#include "../macros/macros.cpp"

template <typename T>
func let
{
public:
    std::unique_ptr<T> data;

    let(T value)
    {
        data = std::make_unique<T>(value);
    }

    T &operator*()
    {
        return *data;
    }

    T *operator->()
    {
        return data.get();
    }

    let &operator=(T value)
    {
        *data = value;
        return *this;
    }
};