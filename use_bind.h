#ifndef _USE_BIND_H_
#define _USE_BIND_H_

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

template <typename iter> void myprinf(iter begin, iter end)
{
    for (auto it = begin; it != end; it++) {
        std::cout << *it << " ";
    }
}

void func1(int age, std::string job, std::string name, char gender)
{
    std::cout << "name is " << name << " age is " << age << " gender is "
              << gender << " job is " << job << std::endl;
}

void func2(int age, std::string name)
{
    std::cout << "name is " << name << " age is " << age << std::endl;
}

void usingbind()
{
    auto newfunc = std::bind(func1, std::placeholders::_1,
                             std::placeholders::_2, "chen", 'M');
    newfunc(32, "famer");
}

void usingbind2()
{
    auto newfunc =
        std::bind(func1, 12, std::placeholders::_1, std::placeholders::_2, 'c');
    newfunc("student", "jane");
}

void usingfunctionobject()
{
    std::function<void(int, std::string)> func(std::bind(
        func1, std::placeholders::_1, std::placeholders::_2, "fangfang", 'c'));
    func(15, "student");
}

std::ostream& myprinf1(std::ostream& os, int val, char c)
{
    os << val << c;
    return os;
}

void addpunt()
{
    std::vector<int> ivec = {1, 2, 3, 4, 5, 6, 7, 8};
    std::ostringstream ss;
    char ch = ',';
    for_each(ivec.begin(), ivec.end(),
             std::bind(myprinf1, std::ref(ss), std::placeholders::_1, ch));
    std::cout << ss.str();
}
#endif