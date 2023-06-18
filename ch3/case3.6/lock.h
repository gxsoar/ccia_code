#include <mutex>
#include <iostream>

class some_big_object {};

void swap(some_big_object& lhs, some_big_object& rhs);

class X {
public:
  X(some_big_object const &sd) : some_detail(sd) {}

  friend void swap(X& lhs, X&rhs) {
    // 在这里需要先判断lhs和rhs不同，否则会递归的对m_进行加锁从而导致死锁的产生
    if (&lhs == &rhs) return;
    // 使用lock同时对其进行加锁
    std::lock(lhs.m_, rhs.m_);
    // 将锁的声明周期交给lock_guard来管理，其中std::adopt_lock使用来表明m_上面已经有锁的存在了
    // 如果不声明adopt_lock，lock_guard 就会尝试再一次对mutex进行加锁，这样会产生死锁
    std::lock_guard<std::mutex> lock_a(lhs.m_, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(rhs.m_, std::adopt_lock);
    swap(lhs.some_detail, rhs.some_detail);
  }
private:
  some_big_object some_detail;
  std::mutex m_;
};