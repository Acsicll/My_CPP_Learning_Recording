#ifndef _IMITATE_CONTAINER_H_
#define _IMITATE_CONTAINER_H_
#include <cstring>
#include <iostream>
#include <memory>

namespace MyQueue {

class base_queue {
 private:
  std::size_t _size;
  std::size_t _head;
  std::size_t _tail;
  int* _data;

 public:
  base_queue(std::size_t size)
      : _size(size + 1), _head(0), _tail(0), _data(new int[_size]) {}
  ~base_queue() { delete[] _data; }
  void push(int& value) {
    if (_tail + 1 == _size) {
      std::cout << "queue is fulled" << std::endl;
      return;
    }
    _data[_tail] = value;
    _tail++;
    std::cout << "push element is success" << std::endl;
  }
  bool empty() const { return _head == _tail; }
  int pop() {
    auto value = front();
    for (std::size_t i = 0; i < _size - 1; i++) {
      _data[i] = _data[i + 1];
    }
    _tail--;
    return value;
  }
  int front() {
    if (empty()) {
      throw std::runtime_error("queue is empty.");
    }
    auto value = _data[_head];
    return value;
  }
};

class circular_queue {
 private:
  std::size_t _head;
  std::size_t _tail;
  std::size_t _size;
  int* _data;

 public:
  circular_queue(size_t size)
      : _size(size + 1), _head(0), _tail(0), _data(new int[_size]) {
    std::cout << "circular_queue struct" << std::endl;
  }
  ~circular_queue() {
    delete[] _data;
    std::cout << "circular_queue destruct" << std::endl;
  }
  bool empty() const { return _head == _tail; }
  void push(int value) {
    if ((_tail + 1) % _size == _head) {
      std::cout << "queue is fulled" << std::endl;
      return;
    }
    _data[_tail] = value;
    _tail = (_tail + 1) % _size;
    std::cout << "push element is successful" << std::endl;
  }
  int front() {
    if (empty()) {
      throw std::runtime_error("circular_queue is empty.");
    }
    auto value = _data[_head];
    return value;
  }
  int pop() {
    auto value = front();
    _head = (_head + 1) % _size;
    return value;
  }
};

class variable_length_queue {
 private:
  std::size_t _head;
  std::size_t _tail;
  std::size_t _size;
  std::size_t _capacity;
  int* _data;

 public:
  explicit variable_length_queue()
      : _head(0), _tail(0), _size(1), _capacity(1), _data(nullptr) {
    std::cout << "variable_length_queue construct" << std::endl;
  }
  ~variable_length_queue() {
    if (_data != nullptr) {
      delete[] _data;
    }
    std::cout << "variable_length_queue destruct" << std::endl;
  }
  variable_length_queue(const variable_length_queue& other) {
    // 拷贝构造无需考虑是否与自身相等，只考虑需要深拷贝或浅拷贝
    _head = other._head;
    _tail = other._tail;
    _size = other._size;
    _capacity = other._capacity;
    if (other._data != nullptr) {
      _data = new int[_capacity];
      std::memcpy(_data, other._data, _size * sizeof(int));
    } else {
      _data = nullptr;
    }
    std::cout << "variable_length_queue copy construct" << std::endl;
  }
  variable_length_queue(variable_length_queue&& other) {
    _head = other._head;
    _tail = other._tail;
    _size = other._size;
    _capacity = other._capacity;
    other._head = 0;
    other._size = 0;
    other._tail = 0;
    other._capacity = 0;
    if (other._data != nullptr) {
      _data = new int[_capacity];
      std::memcpy(_data, other._data, _size * sizeof(int));
      other._data = nullptr;
    } else {
      _data = nullptr;
    }
    std::cout << "variable_length_queue move construct" << std::endl;
  }
  variable_length_queue& operator=(const variable_length_queue& other) {
    if (this != &other) {
      _head = other._head;
      _tail = other._tail;
      _capacity = other._capacity;
      if (_data != nullptr) {
        delete[] _data;
      }
      _size = other._size;
      if (other._data != nullptr) {
        _data = new int[_capacity];
        std::memcpy(_data, other._data, (_size * sizeof(int)));
      } else {
        _data = nullptr;
      }
    }
    return *this;
  }
  variable_length_queue& operator=(variable_length_queue&& other) {
    if (this != &other) {
      delete[] _data;
      _head = other._head;
      _tail = other._tail;
      _size = other._size;
      _capacity = other._capacity;
      _data = other._data;
      other._head = 0;
      other._size = 0;
      other._tail = 0;
      other._capacity = 0;
      other._data = nullptr;
    }
    return *this;
  }
  bool empty() const { return _head == _tail; }
  void push(int value) {
    if (_size == _capacity) {
      size_t new_capacity =
          _capacity > 10 ? (_capacity + _capacity / 2) : _capacity * 2;
      auto temp_new_space = new int[new_capacity];
      if (_data != nullptr) {
        std::memcpy(temp_new_space, _data, _size * sizeof(int));
        delete[] _data;
      }
      _capacity = new_capacity;
      _data = temp_new_space;
      temp_new_space = nullptr;
    }
    _data[_tail] = value;
    _tail++;
    _size++;
    std::cout << "push element successful" << std::endl;
  }
  int front() {
    if (empty()) {
      throw std::runtime_error("variable_length_queue is empty.");
    }
    auto value = _data[_head];
    return value;
  }
  int pop() {
    if (empty()) {
      throw std::runtime_error("variable_length_queue is empty.");
    }
    auto value = _data[_head];
    _size--;
    _tail--;
    // std::memcpy(_data+_head,_data+_head+1,_size);
    for (int i = 0; i < _size - 1; i++) {
      _data[i] = _data[i + 1];
    }
    return value;
  }
};

template <typename T, std::size_t N>
class template_circular_queue : private std::allocator<T> {
 private:
  std::size_t _head;
  std::size_t _tail;
  std::size_t _size;
  T* _data;

 public:
  template_circular_queue()
      : _head(0),
        _tail(0),
        _size(N + 1),
        _data(std::allocator<T>::allocate(_size)) {
    std::cout << "template_queue consturct" << std::endl;
  }
  template_circular_queue(const template_circular_queue& other) = delete;
  template_circular_queue(template_circular_queue&& other) = delete;
  ~template_circular_queue() {
    while (_head != _tail) {
      std::allocator<T>::destroy(_data + _head);
      _head = (_head + 1) % _size;
    }
    if (_data != nullptr) {
      std::allocator<T>::deallocate(_data, _size);
    }
    std::cout << "template_queue destruct" << std::endl;
  }
};

}  // namespace MyQueue

#endif