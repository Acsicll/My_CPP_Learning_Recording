// #include <stdlib.h>
#include <climits>
#include <cstddef>
#include <iostream>
#include <new>
#include <vector>

namespace JJ {

template <class T>
inline T* _allocate(ptrdiff_t size, T*) {
  // ptrdiff_t repersent the difference with two pointer
  std::set_new_handler(0);
  // set_new_handler is stander library function that would
  // not execur anything while be setted 0
  T* tmp = static_cast<T*>(::operator new((size_t)(size * sizeof(T))));
  // ::operator new will allocate the memory and return void type pointer
  // the difference with new between ::operator new is that new would
  // allocate the memory and coustruct the object automatecally but not for
  // ::operator new
  if (tmp == nullptr) {
    std::cerr << "out of memory" << std::endl;
    throw std::bad_alloc();
    // exit(1);
  }
  return tmp;
}
template <class T>
inline void _deallocate(T* buffer) {
  ::operator delete(buffer);
}

template <class T1, class T2>
inline void _construct(T1* p, const T2& value) {
  // T1 is the object type that we want to construct
  // T2 is the parameter type that constructing the object needed
  new (p) T1(value);
}

template <class T>
inline void _destory(T* ptr) {
  ptr->~T;
}

template <class T>
class allocator_ {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  // rebind allocator of type U
  template <class U>
  struct rebind {
    typedef allocator_<U> other;
  };

  // hint used for locality
  pointer allocate(size_type n, const void* hint = 0) {
    return _allocate(static_cast<difference_type>(n), static_cast<pointer>(0));
  }

  void deallocate(pointer p, size_type n) { _deallocate(p); }

  void construct(pointer p, const T& value) { _construct(p, value); }

  void destory(pointer p) { _destory(p); }

  pointer address(reference x) { return static_cast<pointer>(&x); }

  const_pointer const_address(const_reference x) {
    return static_cast<const_pointer>(&x);
  }

  size_type max_size() const {
    return static_cast<size_type>(UINT_MAX / sizeof(T));
  }
};

}  // namespace JJ

int main() {
  int ia[5] = {0, 1, 2, 3, 4};
  unsigned int i;
  std::vector<int, JJ::allocator_<int>> iv(ia, ia + 5);
  for (auto val : iv) {
    std::cout << val << " ";
  }
}