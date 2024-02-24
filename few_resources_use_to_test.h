#ifndef _FEW_RESOURCES_USE_TO_TEST_H_
#define _FEW_RESOURCES_USE_TO_TEST_H_
#include <atomic>
#include <iostream>
#include <memory>
#include <mingw.thread.h>
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
namespace CanNoTRunInVscode {
unsigned const max_hazard_pointers = 100;
struct hazard_pointer
{
    std::atomic<std::thread::id> _id;
    std::atomic<void*> _pointer;
};
hazard_pointer hazard_pointers[max_hazard_pointers];
}  // namespace CanNoTRunInVscode

#endif