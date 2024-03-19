#ifndef _CONCURRENCY_LEANING_H_
#define _CONCURRENCY_LEANING_H_
#include <assert.h>
#include <mingw.condition_variable.h>
#include <mingw.mutex.h>
#include <mingw.shared_mutex.h>
#include <mingw.thread.h>
#include "few_resources_use_to_test.h"
#include "meta_template_learning.h"
// #include <mingw.future.h>
// #include <condition_variable>
// #include <thread>
// #include <shared_mutex>
// #include <mutex>
#include <stdio.h>
#include <algorithm>
#include <atomic>
#include <exception>
#include <future>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <vector>

void test_thread() {
  int i = 0;
  std::thread t1([&]() {
    long num = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    while (num < 100000000) {
      i++;
      num++;
    }
    std::cout << i << std::endl;
  });
  std::thread t2([&]() {
    long num = 0;
    while (num < 100000000) {
      i++;
      num++;
    }
    std::cout << i << std::endl;
  });

  t1.join();
  t2.join();
}

void write_x_then_y() {
  x.store(true, std::memory_order_relaxed);
  y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
  while (!y.load(std::memory_order_relaxed))
    std::cout << "y load false" << std::endl;

  if (x.load(std::memory_order_relaxed)) {
    ++z;
  }
}

void TestOrderRelaxed() {
  std::thread t1(write_x_then_y);
  std::thread t2(read_y_then_x);
  t1.join();
  t2.join();
  // 理论上，assert断言有几率触发:
  // x(true)存入，y(true)存入，
  // y(true)存入memory，
  // y(true)被读，x(false)被读
  assert(z.load() != 0);
}

void write_x_then_y_2() {
  x.store(true, std::memory_order_seq_cst);
  y.store(true, std::memory_order_seq_cst);
}

void read_y_then_x_2() {
  while (!y.load(std::memory_order_seq_cst))
    std::cout << "y load false" << std::endl;

  if (x.load(std::memory_order_seq_cst)) {
    z++;
  }
}

void TestOrderSeqCst() {
  std::thread t1(write_x_then_y_2);
  std::thread t2(read_y_then_x_2);
  t1.join();
  t2.join();
  // 使用全局一致性，既明确只能等一个cpu完成写入操作后才可读：
  // x(true)存入，y(true)存入，
  // x(true)存入memory，y(true)存入memory
  // y(true)被读，x(true)被读
  assert(z.load() != 0);
}

template <typename T, size_t Cap>
class CircularQueueLK : private std::allocator<T> {
 private:
  std::size_t _max_size;
  T* _data;
  std::mutex _mtx;
  std::size_t _head = 0;
  std::size_t _tail = 0;

 public:
  CircularQueueLK()
      : _max_size(Cap + 1),
        _data(std::allocator<T>::allocate(_max_size)),
        _tail(0),
        _head(0) {}
  CircularQueueLK(const CircularQueueLK&) = delete;
  CircularQueueLK& operator=(const CircularQueueLK&) volatile = delete;
  CircularQueueLK& operator=(const CircularQueueLK) = delete;
  ~CircularQueueLK() {
    std::lock_guard<std::mutex> lock(_mtx);
    while (_head != _tail) {
      std::allocator<T>::destroy(_data + _head);
      _head = (_head + 1) % _max_size;
    }
    std::allocator<T>::deallocate(_data, _max_size);
  }
  template <typename... Args>
  bool emplace(Args&&... args) {
    std::lock_guard<std::mutex> lock(_mtx);
    if ((_tail + 1) % _max_size == _head) {
      std::cout << "circular queue full" << std::endl;
      return false;
    }
    std::allocator<T>::construct(_data + _tail, std::forward<Args>(args)...);
    _tail = (_tail + 1) % _max_size;
    return true;
  }
  bool push(T&& val) {
    std::cout << "call push T&& version" << std::endl;
    return emplace(std::move(val));
  }
  bool push(const T& val) {
    std::cout << "call push const T& version" << std::endl;
    return emplace(val);
  }
  bool pop(T& val) {
    std::lock_guard<std::mutex> lock(_mtx);
    if (_head == _tail) {
      std::cout << "circular queue empty" << std::endl;
      return false;
    }
    val = std::move(_data[_head]);
    _head = (_head + 1) % _max_size;
    return true;
  }
};

template <typename T, std::size_t Cap>
class CircularQueueSeq : private std::allocator<T> {
 private:
  std::size_t _head;
  std::size_t _tail;
  std::size_t _size;
  std::atomic<bool> _atomic_using;
  T* _data;

 public:
  CircularQueueSeq()
      : _size(Cap + 1),
        _data(std::allocator<T>::allocate(_size)),
        _head(0),
        _tail(0),
        _atomic_using(false) {}
  CircularQueueSeq(const CircularQueueSeq&) = delete;
  CircularQueueSeq& operator=(const CircularQueueSeq&) volatile = delete;
  CircularQueueSeq& operator=(const CircularQueueSeq&) = delete;
  ~CircularQueueSeq() {
    bool use_expected = false;
    bool use_desired = true;
    do {
      use_expected = false;
      use_desired = true;
    } while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
    while (_head != _tail) {
      std::allocator<T>::destroy(_data + _head);
      _head = (_head + 1) % _size;
    }
    std::allocator<T>::deallocate(_data, _size);
    do {
      use_expected = true;
      use_desired = false;
    } while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
  }
  template <typename... Args>
  bool emplace(Args&&... args) {
    bool use_expected = false;
    bool use_desired = true;
    do {
      use_expected = false;
      use_desired = true;
    } while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
    std::cout << "circular queue doing push" << std::endl;
    if ((_tail + 1) % _size == _head) {
      std::cout << "circular queue full" << std::endl;
      do {
        use_expected = true;
        use_desired = false;
      } while (
          !_atomic_using.compare_exchange_strong(use_expected, use_desired));
      return false;
    }
    std::allocator<T>::construct(_data + _tail, std::forward<Args>(args)...);
    _tail = (_tail + 1) % _size;
    do {
      use_expected = true;
      use_desired = false;
    } while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
    return true;
  }
  bool push(const T& value) {
    std::cout << "called push const T& version" << std::endl;
    return emplace(value);
  }
  bool push(T&& value) {
    std::cout << "called push T&& version" << std::endl;
    return emplace(std::move(value));
  }
  bool pop(T& value) {
    bool use_expected = false;
    bool use_desired = true;
    do {
      use_expected = false;
      use_desired = true;
    } while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
    std::cout << "circular queue doing pop" << std::endl;
    if (_head == _tail) {
      std::cout << "circular queue empty" << std::endl;
      do {
        use_expected = true;
        use_desired = false;
      } while (
          !_atomic_using.compare_exchange_strong(use_expected, use_desired));
      return false;
    }
    value = std::move(_data[_head]);
    _head = (_head + 1) % _size;
    do {
      use_expected = true;
      use_desired = false;
    } while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
    return true;
  }
};

void write_x() {
  x.store(true, std::memory_order_release);
}

void write_y() {
  y.store(true, std::memory_order_release);
}

void read_x_then_y() {
  while (!x.load(std::memory_order_acquire))
    ;
  if (y.load(std::memory_order_acquire)) {
    ++z;
  }
}

void read_y_then_y() {
  while (!y.load(std::memory_order_acquire))
    ;
  if (x.load(std::memory_order_acquire)) {
    ++z;
  }
}

void TestAR() {
  x = false;
  y = false;
  z = 0;
  std::thread a(write_x);
  std::thread b(write_y);
  std::thread c(read_x_then_y);
  std::thread d(read_y_then_x);
  a.join();
  b.join();
  c.join();
  d.join();
  assert(z.load() != 0);
  std::cout << "z value is " << z.load() << std::endl;
}

void write_x_then_y_3() {
  x.store(true, std::memory_order_relaxed);  // 1
  y.store(true,
          std::memory_order_release);  // 2 release promise 1 happen before 2
}

void read_y_then_x_3() {
  while (!y.load(std::memory_order_acquire))
    ;                                       // 3 promise 2 happen before 3
  if (x.load(std::memory_order_relaxed)) {  // 4
    z++;
  }
}

void TestRelAckSeq() {
  x = false;
  y = false;
  z = 0;
  std::thread t1(write_x_then_y_3);
  std::thread t2(read_y_then_x_3);
  t1.join();
  t2.join();
  assert(z.load() != 0);
  std::cout << "z value is " << z.load() << std::endl;
}

void write_x_then_y_3_use_fence() {
  x.store(true, std::memory_order_relaxed);
  // 2 release promises memory to be written before here
  std::atomic_thread_fence(std::memory_order_release);
  y.store(true, std::memory_order_relaxed);
}

void read_y_then_x_3_use_fence() {
  while (!y.load(std::memory_order_relaxed))
    ;
  // 3 fence promises 2 happen before 3
  std::atomic_thread_fence(std::memory_order_acquire);
  if (x.load(std::memory_order_relaxed)) {
    z++;
  }
}

void TestRelAckSeqUseFence() {
  x = false;
  y = false;
  z = 0;
  std::thread t1(write_x_then_y_3_use_fence);
  std::thread t2(read_y_then_x_3_use_fence);
  t1.join();
  t2.join();
  assert(z.load() != 0);
  std::cout << "z value is " << z.load() << std::endl;
}

struct empty_stack : std::exception {
  const char* what() const noexcept override { return "stack empty\n"; }
};
// Defective version :
// Exception is used to show some issuses that are out of predictable.
template <typename T>
class threadsafe_stack {
 private:
  std::stack<T> _data;
  mutable std::mutex _mtx;

 public:
  threadsafe_stack() {}
  threadsafe_stack(const threadsafe_stack& other) {
    std::lock_guard<std::mutex> lock(_mtx);
    _data = other._data;
  }
  threadsafe_stack& operator=(const threadsafe_stack&) = delete;
  void push(T new_value) {
    std::lock_guard<std::mutex> lock(_mtx);
    _data.push(std::move(new_value));
  }
  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lock(_mtx);
    if (_data.empty())
      throw empty_stack();
    std::shared_ptr<T> const res(std::make_shared<T>(std::move(_data.top())));
    _data.pop();
    return res;
  }
  void pop(T& value) {
    std::lock_guard<std::mutex> lock(_mtx);
    if (_data.empty())
      throw empty_stack();
    value = std::move(_data.top());
    _data.pop();
  }
  bool empty() const {
    std::lock_guard<std::mutex> lock(_mtx);
    return _data.empty();
  }
};
// Fixed version :
template <typename T>
class threadsafe_stack_waitable {
 private:
  std::stack<T> _data;
  mutable std::mutex _mtx;
  std::condition_variable _cv;

 public:
  threadsafe_stack_waitable() {}
  threadsafe_stack_waitable(const threadsafe_stack_waitable& other) {
    std::lock_guard<std::mutex> lock(_mtx);
    _data = other._data;
  }
  threadsafe_stack_waitable& operator=(const threadsafe_stack_waitable&) =
      delete;
  void push(T new_value) {
    std::lock_guard<std::mutex> lock(_mtx);
    _data.push(std::move(new_value));
    _cv.notify_one();
  }
  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.wait(lock, [this]() {
      if (_data.empty()) {
        return false;
      }
      return true;
    });
    std::shared_ptr<T> const res(std::make_shared<T>(std::move(_data.top())));
    _data.pop();
    return res;
  }
  void wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.wait(lock, [this]() {
      if (_data.empty()) {
        return false;
      }
      return true;
    });
    value = std::move(_data.top());
    _data.pop();
  }
  bool empty() const {
    std::lock_guard<std::mutex> lock(_mtx);
    return _data.empty();
  }
  bool try_pop(T& value) {
    std::lock_guard<std::mutex> lock(_mtx);
    if (_data.empty()) {
      return false;
    }
    value = std::move(_data.top());
    _data.pop();
    return true;
  }
  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lock(_mtx);
    if (_data.empty()) {
      return std::shared_ptr<T>();
    }
    std::shared_ptr<T> res(std::make_shared<T>(std::move(_data.top())));
    _data.pop();
    return res;
  }
};
// Defecetive version :
// It will create bad data when the smart pointers construct faild.
template <typename T>
class threadsafe_queue {
 private:
  mutable std::mutex _mtx;
  std::queue<T> _data_queue;
  std::condition_variable _cv;

 public:
  threadsafe_queue() {}
  void push(T new_value) {
    std::lock_guard<std::mutex> lock(_mtx);
    _data_queue.push(std::move(new_value));
    _cv.notify_one();
  }
  void wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.wait(lock, [this]() { return !_data_queue.empty(); });
    value = std::move(_data_queue.front());
    _data_queue.pop();
  }
  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.wait(lock, [this]() { return !_data_queue.empty(); });
    std::shared_ptr<T> res(std::make_shared<T>(std::move(_data_queue.front())));
    _data_queue.pop();
    return res;
  }
  bool try_pop(T& value) {
    std::lock_guard<std::mutex> lock(_mtx);
    if (_data_queue.empty()) {
      return false;
    }
    value = std::move(_data_queue.front());
    _data_queue.pop();
    return true;
  }
  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lock(_mtx);
    if (_data_queue.empty()) {
      return false;
    }
    std::shared_ptr<T> const res(
        std::make_shared<T>(std::move(_data_queue.front())));
    _data_queue.pop();
    return res;
  }
  bool empty() const {
    std::lock_guard<std::mutex> lock(_mtx);
    return _data_queue.empty();
  }
};

// Fixed version :
// Using the smart pointer mange resources.
// No more passing value directly,try to use the smart pointer to store
// resources.
template <typename T>
class threadsafe_queue_ptr {
 private:
  mutable std::mutex _mtx;
  std::queue<std::shared_ptr<T>> _data_queue;
  std::condition_variable _cv;

 public:
  threadsafe_queue_ptr() {}
  void push(T new_value) {
    std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
    std::lock_guard<std::mutex> lock(_mtx);
    _data_queue.push(std::move(new_value));
    _cv.notify_one();
  }
  void wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.wait(lock, [this]() { return !_data_queue.empty(); });
    value = std::move(*_data_queue.front());
    _data_queue.pop();
  }
  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.wait(lock, [this]() { return !_data_queue.empty(); });
    std::shared_ptr<T> res = _data_queue.front();
    _data_queue.pop();
    return res;
  }
  bool try_pop(T& value) {
    std::lock_guard<std::mutex> lock(_mtx);
    if (_data_queue.empty()) {
      return false;
    }
    value = std::move(*_data_queue.front());
    _data_queue.pop();
    return true;
  }
  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lock(_mtx);
    if (_data_queue.empty()) {
      return false;
    }
    std::shared_ptr<T> const res = _data_queue.front();
    _data_queue.pop();
    return res;
  }
  bool empty() const {
    std::lock_guard<std::mutex> lock(_mtx);
    return _data_queue.empty();
  }
};

template <typename T>
class threadsafe_queue_ht {
 private:
  struct node {
    std::shared_ptr<T> _data;
    std::unique_ptr<node> _next;
  };
  std::mutex _head_mutex;
  std::unique_ptr<node> _head_ptr;
  std::mutex _tail_mutex;
  node* _tail_ptr;
  std::condition_variable _cv;

  node* get_tail() {
    std::lock_guard<std::mutex> lock(_tail_mutex);
    return _tail_ptr;
  }
  std::unique_ptr<node> pop_head() {
    std::unique_ptr<node> old_head = std::move(_head_ptr);
    _head_ptr = std::move(old_head->_next);
    return old_head;
  }
  std::unique_lock<std::mutex> wait_for_data() {
    std::unique_lock<std::mutex> lock(_head_mutex);
    _cv.wait(lock, [&]() { return _head_ptr.get() != get_tail(); });
    return std::move(lock);
  }
  std::unique_ptr<node> wait_pop_head() {
    std::unique_lock<std::mutex> lock(wait_for_data());
    return pop_head();
  }
  std::unique_ptr<node> wait_pop_head(T& value) {
    std::unique_lock<std::mutex> lock(wait_for_data());
    value = std::move(*(_head_ptr->_data));
    return pop_head();
  }
  std::unique_ptr<node> try_pop_head() {
    std::lock_guard<std::mutex> lock(_head_mutex);
    if (_head_ptr.get() == get_tail()) {
      return std::unique_ptr<node>();
    }
    return pop_head();
  }
  std::unique_ptr<node> try_pop_head(T& value) {
    std::lock_guard<std::mutex> lock(_head_mutex);
    if (_head_ptr.get() == get_tail()) {
      return std::unique_ptr<node>();
    }
    value = std::move(*(_head_ptr->_data));
    return pop_head();
  }

 public:
  threadsafe_queue_ht() : _head_ptr(new node), _tail_ptr(_head_ptr.get()) {}
  threadsafe_queue_ht(const threadsafe_queue_ht&) = delete;
  threadsafe_queue_ht& operator=(const threadsafe_queue_ht&) = delete;
  std::shared_ptr<T> wait_and_pop() {
    std::unique_ptr<node> const old_head = wait_pop_head();
    return old_head->_data;
  }
  void wait_and_pop(T& value) {
    std::unique_ptr<node> const old_head = wait_pop_head(value);
  }
  std::shared_ptr<T> try_pop() {
    std::unique_ptr<node> old_head = try_pop_head();
    return old_head ? old_head->_data : std::shared_ptr<T>();
  }
  bool try_pop(T& value) {
    std::unique_ptr<node> old_head = try_pop_head(value);
    return (old_head) ? true : false;
  }
  bool empty() {
    std::lock_guard<std::mutex> lock(_head_mutex);
    return (_head_ptr.get() == get_tail());
  }
  void push(T new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
    std::unique_ptr<node> p(new node);
    node* const new_tail = p.get();
    std::lock_guard<std::mutex> lock(_tail_mutex);
    _tail_ptr->_data = new_data;
    _tail_ptr->_next = std::move(p);
    _tail_ptr = new_tail;
  }
};

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lookup_table {
 private:
  class bucket_type {
    friend class threadsafe_lookup_table;

   private:
    typedef std::pair<Key, Value> bucket_value;
    typedef std::list<bucket_value> bucket_data;
    typedef typename bucket_data::iterator bucket_iterator;
    bucket_data _data;
    mutable std::shared_mutex _mutex;

    bucket_iterator find_entry_for(const Key& key) {
      return std::find_if(
          _data.begin(), _data.end(),
          [&](bucket_value const& item) { return item.first == key; });
    }

   public:
    Value value_for(Key const& key, Value const& default_value) {
      std::shared_lock<std::shared_mutex> lock(_mutex);
      auto found_entry = find_entry_for(key);
      return (found_entry == _data.end()) ? default_value : found_entry->second;
    }
    void add_or_update_mapping(Key const& key, Value const& value) {
      std::unique_lock<std::shared_mutex> lock(_mutex);
      auto found_entry = find_entry_for(key);
      if (found_entry == _data.end()) {
        _data.push_back(bucket_value(key, value));
      } else {
        found_entry->second = value;
      }
    }
    void remove_mapping(Key const& key) {
      std::unique_lock<std::shared_mutex> lock(_mutex);
      auto found_entry = find_entry_for(key);
      if (found_entry != _data.end()) {
        _data.erase(found_entry);
      }
    }
  };

 private:
  std::vector<std::unique_ptr<bucket_type>> _buckets;
  Hash _hasher;
  bucket_type& get_bucket(Key const& key) const {
    std::size_t const bucket_index = _hasher(key) % _buckets.size();
    return *(_buckets[bucket_index]);
  }

 public:
  threadsafe_lookup_table(unsigned num_buckets = 19,
                          Hash const& hasher = Hash())
      : _buckets(num_buckets), _hasher(hasher) {
    for (unsigned i = 0; i < num_buckets; ++i) {
      _buckets[i].reset(new bucket_type);
    }
  }
  threadsafe_lookup_table(threadsafe_lookup_table const& other) = delete;
  threadsafe_lookup_table& operator=(threadsafe_lookup_table const& other) =
      delete;
  Value value_for(Key const& key, Value const& default_value = Value()) {
    return get_bucket(key).value_for(key, default_value);
  }
  void add_or_update_mapping(Key const& key, Value const& value) {
    get_bucket(key).add_or_update_mapping(key, value);
  }
  void remove_mapping(Key const& key) { get_bucket(key).remove_mapping(key); }
  std::map<Key, Value> get_map() {
    std::vector<std::unique_lock<std::shared_mutex>> locks;
    for (unsigned i = 0; i < _buckets.size(); ++i) {
      locks.push_back(std::unique_lock<std::shared_mutex>(_buckets[i]->_mutex));
    }
    std::map<Key, Value> res;
    for (unsigned i = 0; i < _buckets.size(); ++i) {
      typename bucket_type::bucket_iterator it = _buckets[i]->_data.begin();
      for (; it != _buckets[i]->_data.end(); ++it) {
        res.insert(*it);
      }
    }
    return res;
  }
};
template <typename T>
class threadsafe_list {
  struct node {
    std::mutex _mtx;
    std::shared_ptr<T> _data;
    std::unique_ptr<node> _next;
    node() : _next() {}
    node(T const& value) : _data(std::make_shared<T>(value)) {}
  };
  node _head;

 public:
  threadsafe_list() {}
  ~threadsafe_list() {
    remove_if([](node const&) { return true; });
  }
  threadsafe_list(threadsafe_list const& other) = delete;
  threadsafe_list& operator=(threadsafe_list const& other) = delete;
  template <typename Predicate>
  void remove_if(Predicate p) {
    node* current = &_head;
    std::unique_lock<std::mutex> lk(_head._mtx);
    while (node* const next = current->_next.get()) {
      std::unique_lock<std::mutex> next_lk(next->_mtx);
      if (p(*next->_data)) {
        std::unique_ptr<node> old_next = std::move(current->_next);
        current->_next = std::move(next->_next);
        next_lk.unlock();
      } else {
        lk.unlock();
        current = next;
        lk = std::move(next_lk);
      }
    }
  }
  template <typename Predicate>
  bool remove_first(Predicate p) {
    node* current = &_head;
    std::unique_lock<std::mutex> lk(_head._mtx);
    while (node* const next = current->_next.get()) {
      std::unique_lock<std::mutex> new_lk(next->_mtx);
      if (p(*next->_data)) {
        std::unique_lock<node> old_next = std::move(current->_next);
        current->_next = std::move(next->_next);
        return true;
      }
      lk.unlock();
      current = next;
      lk = std::move(new_lk);
    }
    return false;
  }
  void push_front(T const& value) {
    std::unique_ptr<node> new_node(new node(value));
    std::lock_guard<std::mutex> lk(_head._mtx);
    new_node->_next = std::move(_head._next);
    _head._next = std::move(new_node);
  }
  template <typename Predicate>
  std::shared_ptr<T> find_first_if(Predicate p) {
    node* current = &_head;
    std::unique_lock<std::mutex> lk(_head._mtx);
    while (node* const next = current->_next.get()) {
      std::unique_lock<std::mutex> next_lk(next->_mtx);
      lk.unlock();
      if (p(*next->_data)) {
        return next->_data;
      }
      current = next;
      lk = std::move(next_lk);
    }
    return std::shared_ptr<T>();
  }
  template <typename Function>
  void for_each(Function f) {
    node* current = &_head;
    std::unique_lock<std::mutex> lk(_head._mtx);
    while (node* const next = current->_next.get()) {
      std::unique_lock<std::mutex> next_lk(next->_mtx);
      lk.unlock();
      f(*next->_data);
      current = next;
      lk = std::move(next_lk);
    }
  }
};

template <typename T>
class double_push_list {
  struct node {
    std::mutex _mtx;
    std::shared_ptr<T> _data;
    std::unique_ptr<node> _next;
    node() : _next() {}
    node(T const& value) : _data(std::make_shared<T>(value)) {}
  };
  node _head;
  node* _last_node_ptr;
  std::mutex _last_ptr_mtx;

 public:
  double_push_list() { _last_node_ptr = &_head; }
  ~double_push_list() {}
  double_push_list(double_push_list const& other) = delete;
  double_push_list& operator=(double_push_list const& other) = delete;
  void push_front(T const& value) {
    std::unique_ptr<node> new_node(new node(value));
    std::lock_guard<std::mutex> lk(_head._mtx);
    new_node->_next = std::move(_head._next);
    _head._next = std::move(new_node);
    if (_head._next->_next == nullptr) {
      std::lock_guard<std::mutex> last_lk(_last_ptr_mtx);
      _last_node_ptr = _head._next.get();
    }
  }
  void push_back(T const& value) {
    std::unique_ptr<node> new_node(new node(value));
    std::lock(_last_node_ptr->_mtx, _last_ptr_mtx);
    std::unique_lock<std::mutex> lk(_last_node_ptr->_mtx, std::adopt_lock);
    std::unique_lock<std::mutex> last_lk(_last_ptr_mtx, std::adopt_lock);
    _last_node_ptr->_next = std::move(new_node);
    _last_node_ptr = _last_node_ptr->_next.get();
  }
  template <typename Function>
  void for_each(Function f) {
    node* current = &_head;
    std::unique_lock<std::mutex> lk(_head._mtx);
    while (node* const next = current->_next.get()) {
      std::unique_lock<std::mutex> next_lk(next->_mtx);
      lk.unlock();
      f(*next->_data);
      current = next;
      lk = std::move(next_lk);
    }
  }
  template <typename Predicate>
  std::shared_ptr<T> find_first_if(Predicate p) {
    node* current = &_head;
    std::unique_lock<std::mutex> lk(_head._mtx);
    while (node* const next = current->_next.get()) {
      std::unique_lock<std::mutex> next_lk(next->_mtx);
      lk.unlock();
      if (p(*next->_data)) {
        return next->_data;
      }
      current = next;
      lk = std::move(next_lk);
    }
    return std::shared_ptr<T>();
  }
  template <typename Predicate>
  void remove_if(Predicate p) {
    node* current = &_head;
    std::unique_lock<std::mutex> lk(_head._mtx);
    while (node* const next = current->_next.get()) {
      std::unique_lock<std::mutex> next_lk(next->_mtx);
      if (p(*next->_data)) {
        std::unique_ptr<node> old_next = std::move(current->_next);
        current->_next = std::move(next->_next);
        if (current->_next == nullptr) {
          std::lock_guard<std::mutex> last_lk(_last_ptr_mtx);
          _last_node_ptr = current;
        }
        next_lk.unlock();
      } else {
        lk.unlock();
        current = next;
        lk = std::move(next_lk);
      }
    }
  }
  template <typename Predicate>
  bool remove_first(Predicate p) {
    node* current = &_head;
    std::unique_lock<std::mutex> lk(_head._mtx);
    while (node* const next = current->_next.get()) {
      std::unique_lock<std::mutex> next_lk(next->_mtx);
      if (p(*next->_data)) {
        std::unique_ptr<node> old_next = std::move(current->_next);
        current->_next = std::move(next->_next);
        if (current->_next == nullptr) {
          std::lock_guard<std::mutex> last_lk(_last_ptr_mtx);
          _last_node_ptr = current;
        }
        next_lk.unlock();
        return true;
      }
      lk.unlock();
      current = next;
      lk = std::move(next_lk);
    }
    return false;
  }
  template <typename Predicate>
  void insert_if(Predicate p, T const& value) {
    node* current = &_head;
    std::unique_lock<std::mutex> lk(_head._mtx);
    while (node* const next = current->_next.get()) {
      std::unique_lock<std::mutex> next_lk(next->_mtx);
      if (p(*next->_data)) {
        std::unique_ptr<node> new_node(new node(value));
        auto old_next = std::move(current->_next);
        new_node->_next = std::move(old_next);
        current->_next = std::move(new_node);
        return;
      }
      lk.unlock();
      current = next;
      lk = std::move(next_lk);
    }
  }
};

template <typename T>
class lock_free_stack {
 private:
  struct node {
    std::shared_ptr<T> _data;
    node* _next;
    node(T const& data) : _data(std::make_shared<T>(data)) {}
  };
  lock_free_stack(const lock_free_stack&) = delete;
  lock_free_stack& operator=(const lock_free_stack&) = delete;
  std::atomic<node*> head;
  std::atomic<node*> to_be_deleted;
  std::atomic<int> threads_in_pop;

 public:
  lock_free_stack() {}
  void push(T const& data) {
    node* const new_node = new node(data);
    new_node->_next = head.load();
    while (!head.compare_exchange_weak(new_node->_next, new_node))
      ;
  }
  std::shared_ptr<T> pop() {
    ++threads_in_pop;
    node* old_head = head.load();
    do {
      old_head = head.load();
      if (old_head == nullptr) {
        --threads_in_pop;
        return nullptr;
      }
    } while (!head.compare_exchange_weak(old_head, old_head->_next));
    std::shared_ptr<T> res;
    if (old_head) {
      res.swap(old_head->_data);
    }
    try_reclaim(old_head);
    return res;
  }
  void try_reclaim(node* old_head) {
    if (threads_in_pop == 1) {
      node* nodes_to_delete = to_be_deleted.exchange(nullptr);
      if (!--threads_in_pop) {
        delete_nodes(nodes_to_delete);
      } else if (nodes_to_delete) {
        chain_pending_nodes(nodes_to_delete);
      }
      delete old_head;
    } else {
      chain_pending_node(old_head);
      --threads_in_pop;
    }
  }
  static void delete_nodes(node* nodes) {
    while (nodes) {
      node* next = nodes->_next;
      delete nodes;
      nodes = next;
    }
  }
  void chain_pending_node(node* n) { chain_pending_nodes(n, n); }
  void chain_pending_nodes(node* first, node* last) {
    last->_next = to_be_deleted;
    while (!to_be_deleted.compare_exchange_weak(last->_next, first))
      ;
  }
  void chain_pending_nodes(node* nodes) {
    node* last = nodes;
    while (node* const next = last->_next) {
      last = next;
    }
    chain_pending_nodes(nodes, last);
  }
};

namespace CanNoTRunInVscode {
extern hazard_pointer hazard_pointers[max_hazard_pointers];
class hp_owner {
 public:
  hp_owner(hp_owner const&) = delete;
  hp_owner operator=(hp_owner const&) = delete;
  hp_owner() : hp(nullptr) { bind_hazard_pointer(); }

  std::atomic<void*>& get_pointer() { return hp->pointer; }

  ~hp_owner() {
    hp->pointer.store(nullptr);
    hp->id.store(std::thread::id());
  }

 private:
  void bind_hazard_pointer() {
    for (unsigned i = 0; i < max_hazard_pointers; ++i) {
      std::thread::id old_id;
      if (hazard_pointers[i].id.compare_exchange_strong(
              old_id, std::this_thread::get_id())) {
        hp = &hazard_pointers[i];
        break;
      }
    }

    if (!hp) {
      throw std::runtime_error("No hazard pointers available");
    }
  }
  hazard_pointer* hp;
};

std::atomic<void*>& get_hazard_pointer_for_current_thread() {
  // 每个线程都具有自己的风险指针 线程本地变量
  thread_local static hp_owner hazzard;
  return hazzard.get_pointer();
}

template <typename T>
class hazard_pointer_stack {
 private:
  // 栈节点
  struct node {
    std::shared_ptr<T> data;
    node* next;
    node(T const& data_) : data(std::make_shared<T>(data_)) {}
  };

  // 待删节点
  struct data_to_reclaim {
    node* data;
    data_to_reclaim* next;
    data_to_reclaim(node* p) : data(p), next(nullptr) {}
    ~data_to_reclaim() { delete data; }
  };

  hazard_pointer_stack(const hazard_pointer_stack&) = delete;
  hazard_pointer_stack& operator=(const hazard_pointer_stack&) = delete;
  std::atomic<node*> head;
  std::atomic<data_to_reclaim*> nodes_to_reclaim;

 public:
  hazard_pointer_stack() {}

  void push(T const& data) {
    node* const new_node = new node(data);  // ⇽-- - 2
    new_node->next = head.load();           // ⇽-- - 3
    while (!head.compare_exchange_weak(new_node->next, new_node))
      ;  // ⇽-- - 4
  }

  bool outstanding_hazard_pointers_for(void* p) {
    for (unsigned i = 0; i < max_hazard_pointers; ++i) {
      if (hazard_pointers[i].pointer.load() == p) {
        return true;
      }
    }
    return false;
  }

  void add_to_reclaim_list(data_to_reclaim* reclaim_node) {
    reclaim_node->next = nodes_to_reclaim.load();
    while (!nodes_to_reclaim.compare_exchange_weak(reclaim_node->next,
                                                   reclaim_node))
      ;
  }

  void reclaim_later(node* old_head) {
    add_to_reclaim_list(new data_to_reclaim(old_head));
  }

  void delete_nodes_with_no_hazards() {
    data_to_reclaim* current = nodes_to_reclaim.exchange(nullptr);
    while (current) {
      data_to_reclaim* const next = current->next;
      if (!outstanding_hazard_pointers_for(current->data)) {
        delete current;
      } else {
        add_to_reclaim_list(current);
      }

      current = next;
    }
  }

  std::shared_ptr<T> pop() {
    // 1 从风险列表中获取一个节点给当前线程
    std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
    node* old_head = head.load();
    do {
      node* temp;
      do {
        temp = old_head;
        hp.store(old_head);
        old_head = head.load();
      }  // 2 如果old_head和temp不等说明head被其他线程更新了，需重试
      while (old_head != temp);
    }  // 3 将当前head更新为old_head->next，如不满足则重试
    while (old_head && !head.compare_exchange_strong(old_head, old_head->next));
    // 4一旦更新了head指针，便将风险指针清零
    hp.store(nullptr);
    std::shared_ptr<T> res;
    if (old_head) {
      res.swap(old_head->data);
      // 5 删除旧有的头节点之前，先核查它是否正被风险指针所指涉
      if (outstanding_hazard_pointers_for(old_head)) {
        // 6 延迟删除
        reclaim_later(old_head);
      } else {
        // 7 删除头部节点
        delete old_head;
      }
      // 8 删除没有风险的节点
      delete_nodes_with_no_hazards();
    }
    return res;
  }
};
}  // namespace CanNoTRunInVscode

template <typename T>
class single_ref_stack {
 public:
  single_ref_stack() {}
  ~single_ref_stack() {
    // 循环出栈
    while (pop())
      ;
  }
  void push(T const& data) {
    auto new_node = ref_node(data);
    new_node._node_ptr->_next = head.load();
    while (!head.compare_exchange_weak(new_node._node_ptr->_next, new_node))
      ;
  }
  std::shared_ptr<T> pop() {
    ref_node old_head = head.load();
    for (;;) {
      // 1 只要执行pop就对引用计数+1并更新到head中
      ref_node new_head;
      do {
        new_head = old_head;
        new_head._ref_count += 1;
      } while (!head.compare_exchange_weak(old_head, new_head));
      old_head = new_head;
      auto* node_ptr = old_head._node_ptr;
      if (node_ptr == nullptr) {
        return std::shared_ptr<T>();
      }
      // 2 比较head和old_head想等则交换否则说明head已经被其他线程更新
      if (head.compare_exchange_strong(old_head, node_ptr->_next)) {
        // 要返回的值
        std::shared_ptr<T> res;
        // 交换智能指针
        res.swap(node_ptr->_data);
        // 增加的数量
        int increase_count = old_head._ref_count - 2;
        if (node_ptr->_dec_count.fetch_add(increase_count) == -increase_count) {
          delete node_ptr;
        }
        return res;
      } else {
        if (node_ptr->_dec_count.fetch_sub(1) == 1) {
          delete node_ptr;
        }
      }
    }
  }

 private:
  struct ref_node;
  struct node {
    // 1 数据域智能指针
    std::shared_ptr<T> _data;
    // 2  下一个节点
    ref_node _next;
    node(T const& data_) : _data(std::make_shared<T>(data_)) {}
    // 减少的数量
    std::atomic<unsigned short> _dec_count;
  };
  struct ref_node {
    // 引用计数
    unsigned short _ref_count;
    node* _node_ptr;
    ref_node(T const& data_) : _node_ptr(new node(data_)), _ref_count(1) {}

    ref_node() : _node_ptr(nullptr), _ref_count(0) {}
  };
  // 头部节点
  std::atomic<ref_node> head;
};

namespace AtomicMemoryOperator {
void populate_queue() {
  unsigned const number_items = 200;
  for (unsigned i = 0; i < number_items; ++i) {
    queue_data.push_back(i);
  }
  count.store(number_items, std::memory_order_release);
  store_finish.store(true, std::memory_order_release);
}

void consume_queue_items() {
  while (true) {
    while (!store_finish.load(std::memory_order_relaxed))
      ;

    int item_index;
    if ((item_index = count.fetch_sub(1, std::memory_order_acquire)) <= 0) {
      return;
    }
    std::cout << "queue_data is" << queue_data[item_index - 1] << std::endl;
  }
}

void TesatReleastSeq() {
  std::thread t1(populate_queue);
  std::thread t2(consume_queue_items);
  std::thread t3(consume_queue_items);
  t1.join();
  t2.join();
  t3.join();
}
}  // namespace AtomicMemoryOperator

template <typename T>
class ref_count_stack {
 private:
  struct count_node;
  struct counted_node_ptr {
    int external_count;
    count_node* ptr;
  };
  struct count_node {
    std::shared_ptr<T> _data;
    std::atomic<int> _internal_count;
    counted_node_ptr next;
    count_node(T const& data)
        : _data(std::make_shared<T>(data)), _internal_count(0) {}
  };
  std::atomic<counted_node_ptr> _head;

 public:
  void increase_head_count(counted_node_ptr& old_head) {
    counted_node_ptr new_counter;
    do {
      new_counter = old_head;
      ++new_counter.external_count;
    } while (!_head.compare_exchange_weak(old_head, new_counter,
                                          std::memory_order_acquire,
                                          std::memory_order_relaxed));
    old_head.external_count = new_counter.external_count;
  }

  std::shared_ptr<T> pop() {
    counted_node_ptr old_head = _head.load();
    for (;;) {
      increase_head_count(old_head);
      count_node* const ptr =
          old_head.ptr;  // ptr is a pointer pointing to old_head
      if (!ptr) {
        return std::shared_ptr<T>();
      }
      if (_head.compare_exchange_strong(old_head, ptr->next,
                                        std::memory_order_relaxed)) {
        std::shared_ptr<T> res;
        res.swap(ptr->_data);
        int const count_increase = old_head.external_count - 2;
        if ((ptr->_internal_count.fetch_add(count_increase,
                                            std::memory_order_release)) ==
            -count_increase) {
          delete ptr;
        }
        return res;
      } else if (ptr->_internal_count.fetch_add(
                     -1, std::memory_order_acquire) == 1) {
        ptr->_internal_count.load(std::memory_order_acquire);
        delete ptr;
      }
    }
  }
  ref_count_stack() {
    counted_node_ptr head_node_ptr;
    head_node_ptr.external_count = 0;
    head_node_ptr.ptr = nullptr;
    _head.store(head_node_ptr);
  }
  ~ref_count_stack() {
    while (pop())
      ;
  }
  void push(T const& data) {
    counted_node_ptr new_node;
    new_node.ptr = new count_node(data);
    new_node.external_count = 1;
    new_node.ptr->next = _head.load();
    while (!_head.compare_exchange_weak(new_node.ptr->next, new_node,
                                        std::memory_order_release,
                                        std::memory_order_relaxed))
      ;
  }
};

template <typename T>
class SinglePopPush {
 private:
  struct node {
    std::shared_ptr<T> _data;
    node* _next;
    node() : _next(nullptr) {}
  };
  std::atomic<node*> head;
  std::atomic<node*> tail;
  node* pop_head() {
    node* const old_head = head.load();
    if (old_head == tail.load()) {
      return nullptr;
    }
    head.store(old_head->_next);
    return old_head;
  }

 public:
  SinglePopPush() : head(new node), tail(head.load()) {}
  SinglePopPush(const SinglePopPush& other) = delete;
  SinglePopPush& operator=(const SinglePopPush& other) = delete;
  ~SinglePopPush() {
    while (node* const old_head = head.load()) {
      head.store(old_head->_next);
      delete old_head;
    }
  }
  std::shared_ptr<T> pop() {
    node* old_head = pop_head();
    if (!old_head) {
      return std::shared_ptr<T>();
    }
    std::shared_ptr<T> const res(old_head->_data);
    delete old_head;
    return res;
  }
  void push(T new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
    node* p = new node;
    node* const old_tail = tail.load();
    old_tail->_data.swap(new_data);
    old_tail->_next = p;
    tail.store(p);
  }
};

template <typename T>
class lock_free_queue {
 private:
  struct node_counter {
    unsigned internal_count : 30;
    // ⇽--- 2
    unsigned external_counters : 2;
  };

  struct node;

  struct counted_node_ptr {
    // 存在破坏trivial class 的风险
    /*bool operator == (const counted_node_ptr& cnp) {
        return (external_count == cnp.external_count && ptr == cnp.ptr);
    }*/

    // 构造初始化各成员
    counted_node_ptr() : external_count(0), ptr(nullptr) {}
    int external_count;
    node* ptr;
  };

  struct node {
    std::atomic<T*> data;
    std::atomic<node_counter> count;
    // ⇽---  1
    std::atomic<counted_node_ptr> next;

    node(int external_count = 2) {
      node_counter new_count;
      new_count.internal_count = 0;
      // ⇽---  4
      new_count.external_counters = external_count;
      count.store(new_count);

      counted_node_ptr node_ptr;
      node_ptr.ptr = nullptr;
      node_ptr.external_count = 0;

      next.store(node_ptr);
    }

    void release_ref() {
      std::cout << "call release ref " << std::endl;
      node_counter old_counter = count.load(std::memory_order_relaxed);
      node_counter new_counter;
      do {
        new_counter = old_counter;
        // 1
        --new_counter.internal_count;
      }
      // 2
      while (!count.compare_exchange_strong(old_counter, new_counter,
                                            std::memory_order_acquire,
                                            std::memory_order_relaxed));
      if (!new_counter.internal_count && !new_counter.external_counters) {
        // 3
        delete this;
        std::cout << "release_ref delete success" << std::endl;
        destruct_count.fetch_add(1);
      }
    }
  };

  std::atomic<counted_node_ptr> head;
  // ⇽--- 1
  std::atomic<counted_node_ptr> tail;

  // ⇽---  1
  void set_new_tail(counted_node_ptr& old_tail,
                    counted_node_ptr const& new_tail) {
    node* const current_tail_ptr = old_tail.ptr;
    // ⇽---  2
    // 此处仅有一个线程能设置tail为new_tail，失败的会更新old_tail为tail的新值
    //  为防止失败的线程重试导致tail被再次更新所以添加了后面的&&判断
    while (!tail.compare_exchange_weak(old_tail, new_tail) &&
           old_tail.ptr == current_tail_ptr)
      ;
    // ⇽---  3
    if (old_tail.ptr == current_tail_ptr)
      // ⇽---  4
      free_external_counter(old_tail);
    else
      // ⇽---  5
      current_tail_ptr->release_ref();
  }

  static void free_external_counter(counted_node_ptr& old_node_ptr) {
    std::cout << "call  free_external_counter " << std::endl;
    node* const ptr = old_node_ptr.ptr;
    int const count_increase = old_node_ptr.external_count - 2;
    node_counter old_counter = ptr->count.load(std::memory_order_relaxed);
    node_counter new_counter;
    do {
      new_counter = old_counter;
      // ⇽---  1
      --new_counter.external_counters;
      // ⇽---  2
      new_counter.internal_count += count_increase;
    }
    // ⇽---  3
    while (!ptr->count.compare_exchange_strong(old_counter, new_counter,
                                               std::memory_order_acquire,
                                               std::memory_order_relaxed));
    if (!new_counter.internal_count && !new_counter.external_counters) {
      // ⇽---  4
      destruct_count.fetch_add(1);
      std::cout << "free_external_counter delete success" << std::endl;
      delete ptr;
    }
  }

  static void increase_external_count(std::atomic<counted_node_ptr>& counter,
                                      counted_node_ptr& old_counter) {
    counted_node_ptr new_counter;
    do {
      new_counter = old_counter;
      ++new_counter.external_count;
    } while (!counter.compare_exchange_strong(old_counter, new_counter,
                                              std::memory_order_acquire,
                                              std::memory_order_relaxed));
    old_counter.external_count = new_counter.external_count;
  }

 public:
  lock_free_queue() {
    counted_node_ptr new_next;
    new_next.ptr = new node();
    new_next.external_count = 1;
    tail.store(new_next);
    head.store(new_next);
    std::cout << "new_next.ptr is " << new_next.ptr << std::endl;
  }

  ~lock_free_queue() {
    while (pop())
      ;
    auto head_counted_node = head.load();
    delete head_counted_node.ptr;
  }

  void push(T new_value) {
    std::unique_ptr<T> new_data(new T(new_value));
    counted_node_ptr new_next;
    new_next.ptr = new node;
    new_next.external_count = 1;
    counted_node_ptr old_tail = tail.load();
    for (;;) {
      increase_external_count(tail, old_tail);
      T* old_data = nullptr;
      // ⇽---  6
      if (old_tail.ptr->data.compare_exchange_strong(old_data,
                                                     new_data.get())) {
        counted_node_ptr old_next;
        counted_node_ptr now_next = old_tail.ptr->next.load();
        // ⇽---  7
        if (!old_tail.ptr->next.compare_exchange_strong(old_next, new_next)) {
          // ⇽---  8
          delete new_next.ptr;
          new_next = old_next;  // ⇽---  9
        }
        set_new_tail(old_tail, new_next);
        new_data.release();
        break;
      } else  // ⇽---  10
      {
        counted_node_ptr old_next;
        // ⇽--- 11
        if (old_tail.ptr->next.compare_exchange_strong(old_next, new_next)) {
          // ⇽--- 12
          old_next = new_next;
          // ⇽---  13
          new_next.ptr = new node;
        }
        //  ⇽---  14
        set_new_tail(old_tail, old_next);
      }
    }

    construct_count++;
  }

  std::unique_ptr<T> pop() {
    counted_node_ptr old_head = head.load(std::memory_order_relaxed);
    for (;;) {
      increase_external_count(head, old_head);
      node* const ptr = old_head.ptr;
      if (ptr == tail.load().ptr) {
        // 头尾相等说明队列为空，要减少内部引用计数
        ptr->release_ref();
        return std::unique_ptr<T>();
      }
      //  ⇽---  2
      counted_node_ptr next = ptr->next.load();
      if (head.compare_exchange_strong(old_head, next)) {
        T* const res = ptr->data.exchange(nullptr);
        free_external_counter(old_head);
        return std::unique_ptr<T>(res);
      }
      ptr->release_ref();
    }
  }

  static std::atomic<int> destruct_count;
  static std::atomic<int> construct_count;
};

template <typename T>
std::atomic<int> lock_free_queue<T>::destruct_count = 0;

template <typename T>
std::atomic<int> lock_free_queue<T>::construct_count = 0;

template <typename T>
struct sorter {
  struct chunk_to_sort {
    std::list<T> data;
    std::promise<std::list<T>> promise;
  };
  threadsafe_stack_waitable<chunk_to_sort> chunks;
  std::vector<std::thread> threads;
  unsigned const max_thread_count;
  std::atomic<bool> end_of_data;
  sorter()
      : max_thread_count(std::thread::hardware_concurrency() - 1),
        end_of_data(false) {}
  ~sorter() {
    end_of_data = true;
    for (unsigned i = 0; i < threads.size(); ++i) {
      threads[i].join();
    }
  }
  void try_sort_chunk() {
    std::shared_ptr<chunk_to_sort> chunk = chunks.try_pop();
    if (chunk) {
#ifdef debug
      std::cout << "current thread is: " << std::this_thread::get_id()
                << std::endl;
#endif
      sort_chunk(chunk);
    }
  }

  // return data
  std::list<T> do_sort(std::list<T>& chunk_data) {
    if (chunk_data.empty()) {
      return chunk_data;
    }

    std::list<T> result;
    result.splice(result.begin(), chunk_data, chunk_data.begin());
    T const& partition_val = *result.begin();
    typename std::list<T>::iterator divide_point =
        std::partition(chunk_data.begin(), chunk_data.end(),
                       [&](T const& val) { return val < partition_val; });
    chunk_to_sort new_lower_chunk;
    // the value lower than the selected one will move to an new list
    new_lower_chunk.data.splice(new_lower_chunk.data.end(), chunk_data,
                                chunk_data.begin(), divide_point);
    std::future<std::list<T>> new_lower = new_lower_chunk.promise.get_future();
    chunks.push(std::move(new_lower_chunk));

    if (threads.size() < max_thread_count) {
      threads.push_back(std::thread(&sorter<T>::sort_thread, this));
    }

    std::list<T> new_higher(do_sort(chunk_data));
    result.splice(result.end(), new_higher);
    while (new_lower.wait_for(std::chrono::seconds(0)) !=
           std::future_status::ready) {
      // it must try to sort the next one constantly and it does not need to
      // wait for current working thread to be ready
      try_sort_chunk();
    }
    result.splice(result.begin(), new_lower.get());
    return result;
  }

  void sort_chunk(std::shared_ptr<chunk_to_sort> const& chunk) {
    chunk->promise.set_value(do_sort(chunk->data));
  }

  void sort_thread() {
    while (!end_of_data) {
      try_sort_chunk();
      // yield will hang up the current thread, and put it behind the same level
      // thread in the thread queue
      std::this_thread::yield();
    }
  }
  // actually,it's just a recursion that puts new partition lists to threads by
  // promise's set_value
  // try_sort_chunk -> sort_chunk -> do_sort( sort_thread ) -> try_sort_chunk
};

// implementing an interface for user to use parallel sort class
template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
  if (input.empty()) {
    return input;
  }
  sorter<T> solution;
  return solution.do_sort(input);
}

template <typename ClassType, typename QueType>
class ActorSingle {
 public:
  static ClassType& GetInstance() {
    static ClassType actorsingle;
    return actorsingle;
  }
  ~ActorSingle() {}
  void PostMsg(const QueType& data) { _que.push(data); }

 protected:
  std::atomic<bool> _bstop;
  threadsafe_queue_ht<QueType> _que;
  std::thread _thread;
  ActorSingle() : _bstop(false) {}
  ActorSingle(const ActorSingle&) = delete;
  ActorSingle(ActorSingle&&) = delete;
  ActorSingle& operator=(const ActorSingle&) = delete;
};

class join_threads {
  std::vector<std::thread>& threads;

 public:
  explicit join_threads(std::vector<std::thread>& thread) : threads(thread) {}
  ~join_threads() {
    for (unsigned i = 0; i < threads.size(); ++i) {
      if (threads[i].joinable()) {
        threads[i].join();
      }
    }
  }
};

template <typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func f) {
  unsigned long const length = std::distance(first, last);
  if (!length) {
    return;
  }
  // data grouping,at least 25 datas per thread
  unsigned long const min_per_thread = 25;
  // dividing one data group for the main thread,other data groups delivered to
  // new threads.
  unsigned long const max_threads =
      (length + min_per_thread - 1) / min_per_thread;
  unsigned long const hardware_threads = std::thread::hardware_concurrency();
  unsigned long const num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
  unsigned long const block_size = length / num_threads;

  // the future object accpects thee function return type to be wrapped in it
  std::vector<std::future<void>> futures(num_threads - 1);
  std::vector<std::thread> threads(num_threads - 1);
  join_threads joiner(threads);
  Iterator block_start = first;
  for (unsigned long i = 0; i < (num_threads - 1); ++i) {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    // package_taks usually be used in async tasks to wrap function
    std::packaged_task<void(void)> task(
        [=]() { std::for_each(block_start, block_end, f); });
    // storing the result when thread working done.
    futures[i] = task.get_future();
    threads[i] = std::thread(std::move(task));
    // updating the start iterator after a task finished
    block_start = block_end;
  }
  // making sure the main thread needs to finish a task
  std::for_each(block_start, last, f);
  for (unsigned long i = 0; i < (num_threads - 1); ++i) {
    futures[i].get();
  }
}

// recursion version: if we do not know the number of threads we want,we can use
// async help us to lanuch async task to applicat threads automatically
using namespace SomeTemplateInstance::TemplateExample;
template <typename Iterator, typename Func>
void async_for_each(Iterator first, Iterator last, Func f) {
  unsigned long const length = std::distance(first, last);
  if (!length) {
    return;
  }
  unsigned long const min_per_thread = 25;
  // if currently the number of threads can finish the task,just use it
  if (length < (2 * min_per_thread)) {
    std::for_each(first, last, f);
  } else {
    // Iterator temp_mid_point;
    // using category = typename
    // std::iterator_traits<Iterator>::iterator_category; if constexpr
    // (std::is_same<std::random_access_iterator_tag,
    //                            category>::value) {
    //   temp_mid_point = first + length / 2;
    // } else {
    //   temp_mid_point = first;
    //   size_t temp = length / 2;
    //   while (temp--) {
    //     temp_mid_point++;
    //   }
    // }
    // Iterator const mid_point = temp_mid_point;
    Iterator const mid_point =
        advance<decltype(length)>(first, last, length / 2);
    std::future<void> first_half =
        std::async(&async_for_each<Iterator, Func>, first, mid_point, f);
    async_for_each(mid_point, last, f);
    first_half.get();
  }
}

template <typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last, MatchType match) {
  struct find_element {
    void operator()(Iterator begin,
                    Iterator end,
                    MatchType match,
                    std::promise<Iterator>* result,
                    std::atomic<bool>* done_flag) {
      try {
        for (; begin != end && !done_flag->load(); ++begin) {
          if (*begin == match) {
            result->set_value(begin);
            done_flag->store(true);
            return;
          }
        }
      } catch (...) {
        try {
          result->set_exception(std::current_exception());
        } catch (...) {
        }
      }
    }
  };
  unsigned long const length = std::distance(first, last);
  if (!length) {
    return last;
  }
  unsigned long const min_per_thread = 25;
  unsigned long const max_threads =
      (length + min_per_thread - 1) / min_per_thread;
  unsigned long const hardware_threads = std::thread::hardware_concurrency();
  unsigned long const num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
  unsigned long const block_size = length / num_threads;
  std::promise<Iterator> result;
  std::atomic<bool> done_flag{false};
  std::vector<std::thread> threads(num_threads - 1);
  {
    join_threads joiner(threads);
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
      Iterator block_end = block_start;
      std::advance(block_end, block_size);
      threads[i] = std::thread(find_element(), block_start, block_end, match,
                               &result, &done_flag);
      block_start = block_end;
    }
    find_element()(block_start, last, match, &result, &done_flag);
  }
  if (!done_flag.load()) {
    return last;
  }
  return result.get_future().get();
}

template <typename Iterator, typename MatchType>
Iterator parallel_find_impl(Iterator first,
                            Iterator last,
                            MatchType match,
                            std::atomic<bool>& done) {
  try {
    unsigned long const length = std::distance(first, last);
    unsigned long const min_per_thread = 25;
    if (length < (2 * min_per_thread)) {
      for (; first != last && !done.load(); ++first) {
        if (*first == match) {
          done = true;
          return first;
        }
      }
      return last;
    } else {
      // Iterator const mid_point = first + (length / 2);
      Iterator const mid_point =
          advance<decltype(length)>(first, last, length / 2);
      std::future<Iterator> async_result =
          std::async(&parallel_find_impl<Iterator, MatchType>, mid_point, last,
                     match, std::ref(done));
      Iterator const direct_result =
          parallel_find_impl(first, mid_point, match, done);
      return (direct_result == mid_point) ? async_result.get() : direct_result;
    }
  } catch (...) {
    done = true;
    throw;
  }
}
template <typename Iterator, typename MatchType>
Iterator parallel_find_async(Iterator first, Iterator last, MatchType match) {
  std::atomic<bool> done(false);
  return parallel_find_impl(first, last, match, done);
}

template <typename Iterator>
void parallel_partial_sum(Iterator first, Iterator last) {
  typedef typename Iterator::value_type value_type;
  struct process_chunk {
    void operator()(Iterator begin,
                    Iterator last,
                    std::future<value_type>* previou_end_value,
                    std::promise<value_type>* end_value) {
      try {
        Iterator end = last;
        ++end;
        std::partial_sum(begin, end, begin);
        if (previou_end_value) {
          value_type addend = previou_end_value->get();
          *last += addend;
          if (end_value) {
            end_value->set_value(*last);
          }
          std::for_each(begin, last,
                        [addend](value_type& item) { item += addend; });
        } else if (end_value) {
          end_value->set_value(*last);
        }
      } catch (...) {
        if (end_value) {
          end_value->set_exception(std::current_exception());
        } else {
          throw;
        }
      }
    }
  };
  unsigned long const length = std::distance(first, last);
  if (!length) {
    return;
  }
  unsigned long const min_per_thread = 25;
  unsigned long const max_threads =
      (length + min_per_thread - 1) / min_per_thread;
  unsigned long const hardware_threads = std::thread::hardware_concurrency();
  unsigned long const num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
  unsigned long const block_size = length / num_threads;
  typedef typename Iterator::value_type value_type;
  std::vector<std::thread> threads(num_threads - 1);
  std::vector<std::promise<value_type>> end_values(num_threads - 1);
  std::vector<std::future<value_type>> previous_end_values;
  previous_end_values.reserve(num_threads - 1);
  join_threads joiner(threads);
  Iterator block_start = first;
  for (unsigned long i = 0; i < (num_threads - 1); ++i) {
    Iterator block_last = block_start;
    std::advance(block_last, block_size - 1);
    threads[i] =
        std::thread(process_chunk(), block_start, block_last,
                    (i != 0) ? &previous_end_values[i - 1] : 0, &end_values[i]);
    block_start = block_last;
    ++block_start;
    previous_end_values.push_back(end_values[i].get_future());
  }
  Iterator final_element = block_start;
  std::advance(final_element, std::distance(block_start, last) - 1);
  process_chunk()(block_start, final_element,
                  (num_threads > 1) ? &previous_end_values.back() : 0, 0);
}

class NoneCopy {
 public:
  ~NoneCopy() {}

 protected:
  NoneCopy() {}

 private:
  NoneCopy(const NoneCopy&) = delete;
  NoneCopy& operator=(const NoneCopy&) = delete;
};

class ThreadPool : public NoneCopy {
  using Task = std::packaged_task<void()>;

 private:
  std::atomic<bool> stop_;
  std::atomic<int> thread_nums_;
  std::queue<Task> tasks_;
  std::vector<std::thread> pool_;
  std::mutex mtx_;
  std::condition_variable cv_;

 public:
  ~ThreadPool() { stop(); }
  static ThreadPool& GetInstance() {
    static ThreadPool ins;
    return ins;
  }

  void start() {
    for (int i = 0; i < thread_nums_; i++) {
      pool_.emplace_back([this]() {
        while (!this->stop_.load()) {
          Task task;
          {
            std::unique_lock<std::mutex> lock(mtx_);
            this->cv_.wait(lock, [this] {
              return this->stop_.load() || !this->tasks_.empty();
            });
            if (this->tasks_.empty()) {
              return;
            }
            task = std::move(this->tasks_.front());
            this->tasks_.pop();
          }
          this->thread_nums_++;
          task();
          this->thread_nums_--;
        }
      });
    }
  }

  void stop() {
    stop_.store(true);
    cv_.notify_all();
    for (auto& val : pool_) {
      if (val.joinable()) {
        std::cout << "join thread" << val.get_id() << std::endl;
        val.join();
      }
    }
  }

  int idThreadCount() { return thread_nums_; }

  template <typename Func, typename... Args>
  auto commiTask(Func&& f, Args&&... args) -> std::future<
      decltype(std::forward<Func>(f)(std::forward<Args>(args)...))> {
    using RetType =
        decltype(std::forward<Func>(f)(std::forward<Args>(args)...));
    if (stop_.load()) {
      return std::future<RetType>{};
    }
    auto task = std::make_shared<std::packaged_task<RetType()>>(
        std::bind(std::forward<Func>(f), std::forward<Args>(args)...));
    std::future<RetType> ret = task->get_future();
    {
      std::lock_guard<std::mutex> lock(mtx_);
      tasks_.emplace([task] { (*task)(); });
    }
    cv_.notify_one();
    return ret;
  }

 private:
  ThreadPool(unsigned int num = std::thread::hardware_concurrency())
      : stop_(false) {
    if (num <= 1) {
      thread_nums_ = 2;
    } else {
      thread_nums_ = num;
    }
    start();
  }
};

template <typename T>
std::list<T> pool_thread_quick_sort(std::list<T> input) {
  if (input.empty()) {
    return input;
  }
  std::list<T> result;
  result.splice(result.begin(), input, input.begin());
  T const& partition_val = *result.begin();
  typename std::list<T>::iterator divide_point =
      std::partition(input.begin(), input.end(),
                     [&](T const& val) { return val < partition_val; });
  std::list<T> new_lower_chunk;
  new_lower_chunk.splice(new_lower_chunk.end(), input, input.begin(),
                         divide_point);
  std::future<std::list<T>> new_lower = ThreadPool::GetInstance().commiTask(
      pool_thread_quick_sort<T>, new_lower_chunk);
  std::list<T> new_higher(pool_thread_quick_sort(input));
  result.splice(result.end(), new_higher);
  result.splice(result.begin(), new_lower.get());
  return result;
}
#endif