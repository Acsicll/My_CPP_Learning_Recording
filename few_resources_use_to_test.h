#ifndef _FEW_RESOURCES_USE_TO_TEST_H_
#define _FEW_RESOURCES_USE_TO_TEST_H_
#include <mingw.thread.h>
#include <atomic>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <vector>
std::atomic<bool> x, y;
std::atomic<int> z;

struct MyClass
{
  int _value;
  MyClass(int value) : _value(value) {}
  MyClass(){};
  friend std::ostream &operator<<(std::ostream &os, MyClass ob)
  {
    os << ob._value;
    return os;
  }
  int GetValue() const { return _value; }
};
namespace CanNoTRunInVscode
{
  unsigned const max_hazard_pointers = 100;
  struct hazard_pointer
  {
    std::atomic<std::thread::id> id;
    std::atomic<void *> pointer;
  };
  hazard_pointer hazard_pointers[max_hazard_pointers];
} // namespace CanNoTRunInVscode

void printValue()
{
  std::cout << std::endl;
}

template <typename T, typename... Args>
void printValue(T val, Args... args)
{
  std::cout << val << " ";
  printValue(args...);
}

std::vector<int> queue_data;
std::atomic<int> count;
std::atomic<bool> store_finish{false};

namespace SomeTemplateInstance
{
  namespace TemplateExample
  {
    template <typename It>
    std::size_t distance(It first, It last)
    {
      using category = typename std::iterator_traits<It>::iterator_category;
      // using if constexpr makes complier pass. if constexpr is evaluated at
      // complier time,whereas if is not.This means that branches can be
      // rejected at complier time,and thus will nerver get compiler.
      if constexpr (std::is_same<std::random_access_iterator_tag,
                                 category>::value)
      {
        // if should be always true when the iterator supports to
        // subtracting or adding operation But if not using if constexpr, it
        // can't pass the complier when It belongs to the STL set.
        return last - first;
      }
      else
      {
        std::size_t result = 0;
        while (first != last)
        {
          ++first;
          ++result;
        }
        return result;
      }
    }
  } // namespace TemplateExample

  // using if/else technique to implement this template
  template <typename T, typename U>
  struct is_same
  {
    static const bool value = false;
  };

  template <typename T>
  struct is_same<T, T>
  {
    static const bool value = true;
  };

  template <typename T>
  struct is_pointer
  {
    static const bool value = false;
  };

  template <typename T>
  struct is_pointer<T *>
  {
    static const bool value = true;
  };

  template <typename T>
  struct remove_const
  {
    using type = T;
  };

  template <typename T>
  struct remove_const<const T>
  {
    using type = T;
  };
} // namespace SomeTemplateInstance

#endif