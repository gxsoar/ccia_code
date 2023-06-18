#include <thread>
#include <iostream>
#include <functional>

void do_something(int val) {
  std::cout << val << "\n";
}

struct func {
  int& i;
  func(int& i_) : i(i_) {}
  void operator() () {
    for (int j = 0; j < 10000; ++ j) {
      do_something(i);
    }
  }
};

void oops() {
  int some_local_state{0};
  func my_func(some_local_state);
  std::thread my_thread(my_func);
  // my_thread.detach(); // 不等待新线程结束
  my_thread.join(); // 等待线程结束在执行oops
  // join一次了就不可能继续join
  if (!my_thread.joinable()) std::cout << "yes\n";
}

int main() {
  oops();
  return 0;
}