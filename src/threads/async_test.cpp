// int main() {
//     // استفاده از ماکرو برای ایجاد یک لامبدا
//     async myAsync = hellofun(5);

//     async myAsync = lm {
//             std::cout << "Hi, Params: Some parameters" << std::endl;
//         };

//     async a = []() { std::cout << "Function a\n"; };
//     async b = []() { std::cout << "Function b\n"; };
//     async c = []() { std::cout << "Function c\n"; };

//     // زنجیره‌ای کردن انتساب‌ها
//     a = b = c; // این خط به ترتیب c به b و سپس b به a انتساب می‌دهد

//     return 0;
// }

// #include<iostream>
// using namespace std;

// // Anonymous Class : Class is not having any name
// typedef class
// {
//     // data member
//     int i;
// public:
//     void setData(int i)
//     {
//         // this pointer is used to differentiate
//         // between data member and formal argument.
//         this->i = i;
//     }
//     void print()
//     {
//         cout << "Value for i :" << this->i << endl;
//     }

// } myClass;      // using typedef give a proper name

// // Driver function
// int main()
// {
//     // multiple objects
//     myClass obj1, obj2;
//     obj1.setData(10);
//     obj1.print();

//     obj2.setData(20);
//     obj2.print();
//     return 0;
// }

// #include <iostream>
// using namespace std;

// class AnonymousClass
// {
//     int i;

// public:
//     // Function to set data
//     void setData(int i)
//     {
//         this->i = i;
//     }

//     // Friend function to overload << for printing
//     friend ostream &operator<<(ostream &out, const AnonymousClass &obj);

//     // Overload << operator to take an integer and set data
//     AnonymousClass &operator<<(int value)
//     {
//         this->setData(value);
//         return *this;
//     }
// } go;

// // Implementation of << operator for printing
// ostream &operator<<(ostream &out, const AnonymousClass &obj)
// {
//     out << "Value for i: " << obj.i;
//     return out;
// }

// // Driver function
// int main()
// {
//     // Using the new << operator to set data and print
//     go << 10;           // Set data using << operator
//     cout << go << endl; // Print the data

//     // You can chain this operation if needed:
//     go << 20 << 30;     // Set data multiple times
//     cout << go << endl; // Final print

//     return 0;
// }

#include <iostream>
#include <functional>  // For std::function
#include <any>        // For std::any
#include <string>

// Declare a global instance of AnonymousClass
class AnonymousClass
{
    std::any value; // To hold any type of value

public:
    // Overload << operator for std::any directly
    AnonymousClass& operator<<(const std::any& val)
    {
        value = val;  // Store the std::any directly
        return *this;  // Return object for chaining
    }

    // Overload << operator for functions returning std::any
    AnonymousClass& operator<<(std::any(*func)())
    {
        value = func();  // Call the function and store the result in 'value'
        return *this;  // Return object for chaining
    }

    // Overload << operator for lambdas returning std::any
    AnonymousClass& operator<<(const std::function<std::any()>& func)
    {
        value = func();  // Call the lambda and store the result in 'value'
        return *this;  // Return object for chaining
    }

    // Print function to show the stored value
    void print() const
    {
        if (value.type() == typeid(int))
            std::cout << "Value (int): " << std::any_cast<int>(value) << std::endl;
        else if (value.type() == typeid(std::string))
            std::cout << "Value (string): " << std::any_cast<std::string>(value) << std::endl;
        else
            std::cout << "Value of unknown type." << std::endl;
    }

    // Friend function to overload << for printing
    friend std::ostream& operator<<(std::ostream& out, const AnonymousClass& obj);
} go;

// Implementation of << operator for printing
std::ostream& operator<<(std::ostream& out, const AnonymousClass& obj)
{
    obj.print(); // Call the print function
    return out;
}

// An example function returning std::any
std::any myFunc()
{
    return 42;  // Return an integer
}

// Another example function returning std::any
std::any myStringFunc()
{
    return std::string("Hello, World!");  // Return a string
}

// Create a global instance of AnonymousClass
// AnonymousClass obj1;

// Driver function
int main()
{
    // Using << operator directly with std::any-returning function
    // obj1 << myFunc(); // Store an int in obj1

    // Using << operator with another std::any-returning function
    // obj1 << myStringFunc(); // Store a string in obj1
    // std::cout << obj1 << std::endl;  // Print the value (should be "Hello, World!")

    go << myFunc();
    std::cout << go << std::endl;  // Print the value (should be 42)

    return 0;
}
