#pragma once

#include <iostream>
#include <thread>

class joining_thread {
public:
  joining_thread() = default;
  template<typename Callable, typename ... Args>
  explicit joining_thread(Callable&& func, Args&&... args) : 
                            t_(std::forward<Callable>(func), std::forward<Args>(args) ...) {}
  
  explicit joining_thread(std::thread t) noexcept : t_(std::move(t)) {}
  
  joining_thread(joining_thread&& rhs) noexcept : t_(std::move(rhs.t_)) {}
  
  joining_thread& operator=(std::thread other) noexcept {
    // 如果原来所属的线程还能join先将其join
    if (joinable()) {
      join();
    }
    t_ = std::move(other);
    return *this;
  }

  ~joining_thread() noexcept {
    if (joinable()) {
      join();
    }
  }

  void swap(joining_thread& rhs) noexcept {
    t_.swap(rhs.t_);
  }

  std::thread::id get_id() const noexcept {
    return t_.get_id();
  } 

  void join() { t_.join(); }

  void detach() { t_.detach(); }

  bool joinable() const noexcept { return t_.joinable(); }

  std::thread& as_thread() noexcept { return t_; }

  const std::thread& as_thread() const noexcept { return t_; }

private:
  std::thread t_;
};
