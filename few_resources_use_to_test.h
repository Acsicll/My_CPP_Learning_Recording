#ifndef _FEW_RESOURCES_USE_TO_TEST_H_
#define _FEW_RESOURCES_USE_TO_TEST_H_
#include <mingw.thread.h>
#include <atomic>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>
std::atomic<bool> x, y;
std::atomic<int> z;

struct MyClass {
  int _value;
  MyClass(int value) : _value(value) {}
  MyClass(){};
  friend std::ostream& operator<<(std::ostream& os, MyClass ob) {
    os << ob._value;
    return os;
  }
  int GetValue() const { return _value; }
};
namespace CanNoTRunInVscode {
unsigned const max_hazard_pointers = 100;
struct hazard_pointer {
  std::atomic<std::thread::id> id;
  std::atomic<void*> pointer;
};
hazard_pointer hazard_pointers[max_hazard_pointers];
}  // namespace CanNoTRunInVscode

void printValue() {
  std::cout << std::endl;
}

template <typename T, typename... Args>
void printValue(T val, Args... args) {
  std::cout << val << " ";
  printValue(args...);
}

std::vector<int> queue_data;
std::atomic<int> count;
std::atomic<bool> store_finish{false};

namespace show_cplusplus_version {
void show_cplusplus_version() {
#ifdef __cplusplus
  if (__cplusplus == 202203L) {
    std::cout << "C++ version : C++20" << std::endl;
  } else if (__cplusplus == 201703L) {
    std::cout << "C++ version : C++17" << std::endl;
  } else if (__cplusplus == 201402L) {
    std::cout << "C++ version : C++14" << std::endl;
  } else if (__cplusplus == 201103L) {
    std::cout << "C++ version : C++11" << std::endl;
  } else if (__cplusplus == 199711L) {
    std::cout << "C++ version : C++98" << std::endl;
  } else {
    std::cout << "C++ version : C++(Unkown version)" << std::endl;
  }
#else
  std::cout << "C++ version : C++(Unknow version)" << std::endl;
#endif
}
}  // namespace show_cplusplus_version
#endif