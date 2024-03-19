#pragma once
#include <iostream>
#include <thread>
#include <vector>
class joining_thread {
  std::thread t_;

 public:
  joining_thread() noexcept = default;
  template <typename Callable, typename... Args>
  explicit joining_thread(Callable&& func, Args&&... args)
      : t_(std::forward<Callable>(func), std::forward<Args>(args)...) {}
  explicit joining_thread(std::thread&& other) noexcept
      : t_(std::move(other)) {}
  joining_thread(joining_thread&& other) noexcept : t_(std::move(other.t_)) {}
  joining_thread& operator=(joining_thread&& other) noexcept {
    if (t_.joinable()) {
      t_.join();
    }
    t_ = std::move(other.t_);
    return *this;
  }
  joining_thread& operator=(std::thread other) noexcept {
    if (t_.joinable()) {
      t_.join();
    }
    t_ = std::move(other);
    return *this;
  }
  ~joining_thread() noexcept {
    if (t_.joinable()) {
      t_.join();
    }
  }
  void swap(joining_thread& other) noexcept { t_.swap(other.t_); }
  std::thread::id get_id() const noexcept { return t_.get_id(); }
  bool joinable() const noexcept { return t_.joinable(); }
  void join() { t_.join(); }
  void detach() { t_.detach(); }
  std::thread& as_thread() noexcept { return t_; }
  const std::thread& as_thread() const noexcept { return t_; }
};

int main() {
  std::thread t1([] { std::cout << "this is thread 1\n"; });
  std::thread t2([] { std::cout << "this is thread 2\n"; });
  std::thread t3([] { std::cout << "this is thread 3\n"; });
  std::thread t4([] { std::cout << "this is thread 4\n"; });
  std::vector<joining_thread> join_threads;
  join_threads.push_back(joining_thread(std::move(t1)));
  join_threads.push_back(joining_thread(std::move(t2)));
  join_threads.push_back(joining_thread(std::move(t3)));
  join_threads.push_back(joining_thread(std::move(t4)));
}