#include <mutex>
#include <iostream>

class some_big_object {};

void swap(some_big_object& lhs, some_big_object& rhs);

class X {
public:
  X(some_big_object const &sd) : some_detail(sd) {}

  friend void swap(X& lhs, X&rhs) {
    if (&lhs == &rhs) return;
    std::scoped_lock gurad(lhs.m_, rhs.m_);
    swap(lhs.some_detail, rhs.some_detail);
  }
private:
  some_big_object some_detail;
  std::mutex m_;
};