#ifndef _FEW_RESOURCES_USE_TO_TEST_H_
#define _FEW_RESOURCES_USE_TO_TEST_H_
#include <atomic>
#include <iostream>
#include <memory>
#include <sstream>
std::atomic<bool> x, y;
std::atomic<int> z;

struct MyClass
{
    int _value;
    MyClass(int value) : _value(value) {}
    MyClass(){};
    friend std::ostream& operator<<(std::ostream& os, MyClass ob)
    {
        os << ob._value;
        return os;
    }
    int GetValue() const
    {
        return _value;
    }
};

#endif