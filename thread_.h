#ifndef _THREAD__H_
#define _THREAD__H_
#include "few_resources_use_to_test.h"
#include <algorithm>
#include <assert.h>
#include <atomic>
#include <exception>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <mingw.condition_variable.h>
#include <mingw.mutex.h>
#include <mingw.shared_mutex.h>
#include <mingw.thread.h>
#include <queue>
#include <stack>
#include <vector>

void test_thread()
{
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

void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed))
        std::cout << "y load false" << std::endl;

    if (x.load(std::memory_order_relaxed)) {
        ++z;
    }
}

void TestOrderRelaxed()
{
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

void write_x_then_y_2()
{
    x.store(true, std::memory_order_seq_cst);
    y.store(true, std::memory_order_seq_cst);
}

void read_y_then_x_2()
{
    while (!y.load(std::memory_order_seq_cst))
        std::cout << "y load false" << std::endl;

    if (x.load(std::memory_order_seq_cst)) {
        z++;
    }
}

void TestOrderSeqCst()
{
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
        : _max_size(Cap + 1), _data(std::allocator<T>::allocate(_max_size)),
          _tail(0), _head(0)
    {
    }
    CircularQueueLK(const CircularQueueLK&) = delete;
    CircularQueueLK& operator=(const CircularQueueLK&) volatile = delete;
    CircularQueueLK& operator=(const CircularQueueLK) = delete;
    ~CircularQueueLK()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        while (_head != _tail) {
            std::allocator<T>::destroy(_data + _head);
            _head = (_head + 1) % _max_size;
        }
        std::allocator<T>::deallocate(_data, _max_size);
    }
    template <typename... Args>
    bool emplace(Args&&... args)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if ((_tail + 1) % _max_size == _head) {
            std::cout << "circular queue full" << std::endl;
            return false;
        }
        std::allocator<T>::construct(_data + _tail,
                                     std::forward<Args>(args)...);
        _tail = (_tail + 1) % _max_size;
        return true;
    }
    bool push(T&& val)
    {
        std::cout << "call push T&& version" << std::endl;
        return emplace(std::move(val));
    }
    bool push(const T& val)
    {
        std::cout << "call push const T& version" << std::endl;
        return emplace(val);
    }
    bool pop(T& val)
    {
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
        : _size(Cap + 1), _data(std::allocator<T>::allocate(_size)), _head(0),
          _tail(0), _atomic_using(false)
    {
    }
    CircularQueueSeq(const CircularQueueSeq&) = delete;
    CircularQueueSeq& operator=(const CircularQueueSeq&) volatile = delete;
    CircularQueueSeq& operator=(const CircularQueueSeq&) = delete;
    ~CircularQueueSeq()
    {
        bool use_expected = false;
        bool use_desired = true;
        do {
            use_expected = false;
            use_desired = true;
        } while (
            !_atomic_using.compare_exchange_strong(use_expected, use_desired));
        while (_head != _tail) {
            std::allocator<T>::destroy(_data + _head);
            _head = (_head + 1) % _size;
        }
        std::allocator<T>::deallocate(_data, _size);
        do {
            use_expected = true;
            use_desired = false;
        } while (
            !_atomic_using.compare_exchange_strong(use_expected, use_desired));
    }
    template <typename... Args>
    bool emplace(Args&&... args)
    {
        bool use_expected = false;
        bool use_desired = true;
        do {
            use_expected = false;
            use_desired = true;
        } while (
            !_atomic_using.compare_exchange_strong(use_expected, use_desired));
        std::cout << "circular queue doing push" << std::endl;
        if ((_tail + 1) % _size == _head) {
            std::cout << "circular queue full" << std::endl;
            do {
                use_expected = true;
                use_desired = false;
            } while (!_atomic_using.compare_exchange_strong(use_expected,
                                                            use_desired));
            return false;
        }
        std::allocator<T>::construct(_data + _tail,
                                     std::forward<Args>(args)...);
        _tail = (_tail + 1) % _size;
        do {
            use_expected = true;
            use_desired = false;
        } while (
            !_atomic_using.compare_exchange_strong(use_expected, use_desired));
        return true;
    }
    bool push(const T& value)
    {
        std::cout << "called push const T& version" << std::endl;
        return emplace(value);
    }
    bool push(T&& value)
    {
        std::cout << "called push T&& version" << std::endl;
        return emplace(std::move(value));
    }
    bool pop(T& value)
    {
        bool use_expected = false;
        bool use_desired = true;
        do {
            use_expected = false;
            use_desired = true;
        } while (
            !_atomic_using.compare_exchange_strong(use_expected, use_desired));
        std::cout << "circular queue doing pop" << std::endl;
        if (_head == _tail) {
            std::cout << "circular queue empty" << std::endl;
            do {
                use_expected = true;
                use_desired = false;
            } while (!_atomic_using.compare_exchange_strong(use_expected,
                                                            use_desired));
            return false;
        }
        value = std::move(_data[_head]);
        _head = (_head + 1) % _size;
        do {
            use_expected = true;
            use_desired = false;
        } while (
            !_atomic_using.compare_exchange_strong(use_expected, use_desired));
        return true;
    }
};

void write_x()
{
    x.store(true, std::memory_order_release);
}

void write_y()
{
    y.store(true, std::memory_order_release);
}

void read_x_then_y()
{
    while (!x.load(std::memory_order_acquire))
        ;
    if (y.load(std::memory_order_acquire)) {
        ++z;
    }
}

void read_y_then_y()
{
    while (!y.load(std::memory_order_acquire))
        ;
    if (x.load(std::memory_order_acquire)) {
        ++z;
    }
}

void TestAR()
{
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

void write_x_then_y_3()
{
    x.store(true, std::memory_order_relaxed);  // 1
    y.store(true,
            std::memory_order_release);  // 2 release promise 1 happen before 2
}

void read_y_then_x_3()
{
    while (!y.load(std::memory_order_acquire))
        ;                                     // 3 promise 2 happen before 3
    if (x.load(std::memory_order_relaxed)) {  // 4
        z++;
    }
}

void TestRelAckSeq()
{
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

void write_x_then_y_3_use_fence()
{
    x.store(true, std::memory_order_relaxed);
    // 2 release promises memory to be written before here
    std::atomic_thread_fence(std::memory_order_release);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x_3_use_fence()
{
    while (!y.load(std::memory_order_relaxed))
        ;
    // 3 fence promises 2 happen before 3
    std::atomic_thread_fence(std::memory_order_acquire);
    if (x.load(std::memory_order_relaxed)) {
        z++;
    }
}

void TestRelAckSeqUseFence()
{
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

struct empty_stack : std::exception
{
    const char* what() const noexcept override
    {
        return "stack empty\n";
    }
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
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _data = other._data;
    }
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _data.push(std::move(new_value));
    }
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_data.empty())
            throw empty_stack();
        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(_data.top())));
        _data.pop();
        return res;
    }
    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_data.empty())
            throw empty_stack();
        value = std::move(_data.top());
        _data.pop();
    }
    bool empty() const
    {
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
    threadsafe_stack_waitable(const threadsafe_stack_waitable& other)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _data = other._data;
    }
    threadsafe_stack_waitable&
    operator=(const threadsafe_stack_waitable&) = delete;
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _data.push(std::move(new_value));
        _cv.notify_one();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]() {
            if (_data.empty()) {
                return false;
            }
            return true;
        });
        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(_data.top())));
        _data.pop();
        return res;
    }
    void wait_and_pop(T& value)
    {
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
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(_mtx);
        return _data.empty();
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_data.empty()) {
            return false;
        }
        value = std::move(_data.top());
        _data.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
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
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _data_queue.push(std::move(new_value));
        _cv.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]() { return !_data_queue.empty(); });
        value = std::move(_data_queue.front());
        _data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]() { return !_data_queue.empty(); });
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(_data_queue.front())));
        _data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_data_queue.empty()) {
            return false;
        }
        value = std::move(_data_queue.front());
        _data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_data_queue.empty()) {
            return false;
        }
        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(_data_queue.front())));
        _data_queue.pop();
        return res;
    }
    bool empty() const
    {
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
    threadsafe_queue() {}
    void push(T new_value)
    {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
        std::lock_guard<std::mutex> lock(_mtx);
        _data_queue.push(std::move(new_value));
        _cv.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]() { return !_data_queue.empty(); });
        value = std::move(*_data_queue.front());
        _data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]() { return !_data_queue.empty(); });
        std::shared_ptr<T> res = _data_queue.front();
        _data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_data_queue.empty()) {
            return false;
        }
        value = std::move(*_data_queue.front());
        _data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_data_queue.empty()) {
            return false;
        }
        std::shared_ptr<T> const res = _data_queue.front();
        _data_queue.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(_mtx);
        return _data_queue.empty();
    }
};

template <typename T>
class threadsafe_queue_ht {
  private:
    struct node
    {
        std::shared_ptr<T> _data;
        std::unique_ptr<node> _next;
    };
    std::mutex _head_mutex;
    std::unique_ptr<node> _head_ptr;
    std::mutex _tail_mutex;
    node* _tail_ptr;
    std::condition_variable _cv;

    node* get_tail()
    {
        std::lock_guard<std::mutex> lock(_tail_mutex);
        return _tail_ptr;
    }
    std::unique_ptr<node> pop_head()
    {
        std::unique_ptr<node> old_head = std::move(_head_ptr);
        _head_ptr = std::move(old_head->_next);
        return old_head;
    }
    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock<std::mutex> lock(_head_mutex);
        _cv.wait(lock, [&]() { return _head_ptr.get() != get_tail(); });
        return std::move(lock);
    }
    std::unique_ptr<node> wait_pop_head()
    {
        std::unique_lock<std::mutex> lock(wait_for_data());
        return pop_head();
    }
    std::unique_ptr<node> wait_pop_head(T& value)
    {
        std::unique_lock<std::mutex> lock(wait_for_data());
        value = std::move(*(_head_ptr->_data));
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head()
    {
        std::lock_guard<std::mutex> lock(_head_mutex);
        if (_head_ptr.get() == get_tail()) {
            return std::unique_ptr<node>();
        }
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head(T& value)
    {
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
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_ptr<node> const old_head = wait_pop_head();
        return old_head->_data;
    }
    void wait_and_pop(T& value)
    {
        std::unique_ptr<node> const old_head = wait_pop_head(value);
    }
    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = try_pop_head();
        return old_head ? old_head->_data : std::shared_ptr<T>();
    }
    bool try_pop(T& value)
    {
        std::unique_ptr<node> old_head = try_pop_head(value);
        return (old_head) ? true : false;
    }
    bool empty()
    {
        std::lock_guard<std::mutex> lock(_head_mutex);
        return (_head_ptr.get() == get_tail());
    }
    void push(T new_value)
    {
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

        bucket_iterator find_entry_for(const Key& key)
        {
            return std::find_if(
                _data.begin(), _data.end(),
                [&](bucket_value const& item) { return item.first == key; });
        }

      public:
        Value value_for(Key const& key, Value const& default_value)
        {
            std::shared_lock<std::shared_mutex> lock(_mutex);
            auto found_entry = find_entry_for(key);
            return (found_entry == _data.end()) ? default_value :
                                                  found_entry->second;
        }
        void add_or_update_mapping(Key const& key, Value const& value)
        {
            std::unique_lock<std::shared_mutex> lock(_mutex);
            auto found_entry = find_entry_for(key);
            if (found_entry == _data.end()) {
                _data.push_back(bucket_value(key, value));
            }
            else {
                found_entry->second = value;
            }
        }
        void remove_mapping(Key const& key)
        {
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
    bucket_type& get_bucket(Key const& key) const
    {
        std::size_t const bucket_index = _hasher(key) % _buckets.size();
        return *(_buckets[bucket_index]);
    }

  public:
    threadsafe_lookup_table(unsigned num_buckets = 19,
                            Hash const& hasher = Hash())
        : _buckets(num_buckets), _hasher(hasher)
    {
        for (unsigned i = 0; i < num_buckets; ++i) {
            _buckets[i].reset(new bucket_type);
        }
    }
    threadsafe_lookup_table(threadsafe_lookup_table const& other) = delete;
    threadsafe_lookup_table&
    operator=(threadsafe_lookup_table const& other) = delete;
    Value value_for(Key const& key, Value const& default_value = Value())
    {
        return get_bucket(key).value_for(key, default_value);
    }
    void add_or_update_mapping(Key const& key, Value const& value)
    {
        get_bucket(key).add_or_update_mapping(key, value);
    }
    void remove_mapping(Key const& key)
    {
        get_bucket(key).remove_mapping(key);
    }
    std::map<Key, Value> get_map()
    {
        std::vector<std::unique_lock<std::shared_mutex>> locks;
        for (unsigned i = 0; i < _buckets.size(); ++i) {
            locks.push_back(
                std::unique_lock<std::shared_mutex>(_buckets[i]->_mutex));
        }
        std::map<Key, Value> res;
        for (unsigned i = 0; i < _buckets.size(); ++i) {
            typename bucket_type::bucket_iterator it =
                _buckets[i]->_data.begin();
            for (; it != _buckets[i]->_data.end(); ++it) {
                res.insert(*it);
            }
        }
        return res;
    }
};
template <typename T>
class threadsafe_list {
    struct node
    {
        std::mutex _mtx;
        std::shared_ptr<T> _data;
        std::unique_ptr<node> _next;
        node() : _next() {}
        node(T const& value) : _data(std::make_shared<T>(value)) {}
    };
    node _head;

  public:
    threadsafe_list() {}
    ~threadsafe_list()
    {
        remove_if([](node const&) { return true; });
    }
    threadsafe_list(threadsafe_list const& other) = delete;
    threadsafe_list& operator=(threadsafe_list const& other) = delete;
    template <typename Predicate>
    void remove_if(Predicate p)
    {
        node* current = &_head;
        std::unique_lock<std::mutex> lk(_head._mtx);
        while (node* const next = current->_next.get()) {
            std::unique_lock<std::mutex> next_lk(next->_mtx);
            if (p(*next->_data)) {
                std::unique_ptr<node> old_next = std::move(current->_next);
                current->_next = std::move(next->_next);
                next_lk.unlock();
            }
            else {
                lk.unlock();
                current = next;
                lk = std::move(next_lk);
            }
        }
    }
    void push_front(T const& value)
    {
        std::unique_ptr<node> new_node(new node(value));
        std::lock_guard<std::mutex> lk(_head._mtx);
        new_node->_next = std::move(_head._next);
        _head._next = std::move(new_node);
    }
    template <typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate p)
    {
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
    void for_each(Function f)
    {
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

#endif