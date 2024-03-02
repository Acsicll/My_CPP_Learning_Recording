#ifndef _META_TEMPLATE_LEARNING_H_
#define _META_TEMPLATE_LEARNING_H_
#include <iostream>
#include <iterator>
#include <numeric>
#include <type_traits>
namespace SomeTemplateInstance {
namespace TemplateExample {
template <typename Iterator>
std::size_t distance(Iterator first, Iterator last) {
  using category = typename std::iterator_traits<Iterator>::iterator_category;
  // using if constexpr makes complier pass. if constexpr is evaluated at
  // complier time,whereas if is not.This means that branches can be
  // rejected at complier time,and thus will nerver get compiler.
  if constexpr (std::is_same<std::random_access_iterator_tag,
                             category>::value) {
    // if should be always true when the iterator supports to
    // subtracting or adding operation But if not using if constexpr, it
    // can't pass the complier when It belongs to the STL set.
    return last - first;
  } else {
    std::size_t result = 0;
    while (first != last) {
      ++first;
      ++result;
    }
    return result;
  }
}

template <typename T = std::size_t, typename Iterator>
Iterator advance(Iterator begin, Iterator end, T distance) {
  using category = typename std::iterator_traits<Iterator>::iterator_category;
  if constexpr (std::is_same<std::random_access_iterator_tag,
                             category>::value) {
    size_t length = end - begin;
    if (distance < length && distance >= 0) {
      return begin + distance;
    }
    std::cerr << "error distance to advance" << std::endl;
  } else {
    auto temp = distance;
    auto temp_start = begin;
    while (temp--) {
      ++temp_start;
    }
    return temp_start;
  }
}
}  // namespace TemplateExample

// using if/else technique to implement this templates
template <typename T, typename U>
struct is_same {
  static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
  static const bool value = true;
};

template <typename T>
struct is_pointer {
  static const bool value = false;
};

template <typename T>
struct is_pointer<T*> {
  static const bool value = true;
};

template <typename T>
struct remove_const {
  using type = T;
};

template <typename T>
struct remove_const<const T> {
  using type = T;
};

// giving the first type that is assigned but not the second type which can be
// derivated.
template <typename DstT, typename SrcT>
DstT c_style_cast(SrcT value) {
  return (DstT)(value);
}

template <typename T, std::size_t size>
struct My_Array {
  T data[size];
  T& operator[](std::size_t index) {
    if (index < 0 || index >= 100) {
      std::cerr << "error index size, it should be in the range of 0 to 100"
                << std::endl;
      static T dummy;
      return dummy;
    }
    return data[index];
  }
};

template <typename T>
T max(const T& a, const T& b) {
  return a > b ? a : b;
}
// using std::string_literals allows adding a 's' behind the const char* type
// constant, which means conversion the constant to a string type value
using namespace std::string_literals;

// T1{} constructs a non-name object which can as a initial value
template <typename T1, typename T2, typename RT = decltype(true ? T1{} : T2{})>
RT disable_arr_template_max(const T1& a, const T2& b) {
  return a > b ? a : b;
}

template <typename T1, typename T2>
auto enable_arr_template_max(const T1& a, const T2& b)
    -> decltype(true ? a : b) {
  return a > b ? a : b;
}

//(experssion1,experssion2) will return the value from experssion2 to array
template <typename... Args>
void using_arr_create_template_print(const Args&... args) {
  int dummy[]{0, (std::cout << args << ' ', 0)...};
}

template <typename... Args>
void using_temp_arr_create_template_print(const Args&... args) {
  // declaring  a temp array
  using Arr = int[];
  Arr{0, (std::cout << args << ' ', 0)...};
}

template <typename T, std::size_t N, typename... Args>
void print_arr_by_indexs(const T (&arr)[N], Args... index) {
  using_temp_arr_create_template_print(arr[index]...);
}

template <std::size_t... N>
void non_type_template_print() {
  std::size_t arr[]{N...};
  std::for_each(std::begin(arr), std::end(arr),
                [](std::size_t value) { std::cout << value << ' '; });
}

template <typename... Args, typename RT = std::common_type_t<Args...>>
RT implicit_conversion_ret_type_sum(const Args&... args) {
  RT arr[]{args...};
  //   RT res{};
  //   for (unsigned i = 0; i < sizeof...(args); ++i) {
  //     res += arr[i];
  //   }
  return std::accumulate(std::begin(arr), std::end(arr), RT{});
}

void printValue() {
  std::cout << std::endl;
}

template <typename T, typename... Args>
void printValue(T val, Args... args) {
  std::cout << val << " ";
  printValue(args...);
}

template <typename T, typename C>
void printValue(const C& container) {
  auto start = container.cbegin();
  auto end = container.cend();
  for (; start != end; ++start) {
    std::cout << *start << " ";
  }
}

template <typename C>
void printValue(const C& container) {
  // when it be used,it should be include the algorithm header file
  typename C::const_iterator start = container.cbegin();
  typename C::const_iterator end = container.cend();
  for (; start != end; ++start) {
    std::cout << *start << " ";
  }
}

}  // namespace SomeTemplateInstance

#endif
