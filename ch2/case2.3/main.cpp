#include <thread>
#include <iostream>
#include <functional>
#include <assert.h>
// 采用raii的性质进行线程的退出
class thread_guard {
public: 
  explicit thread_guard(std::thread& t) : t_(t) {}
  ~thread_guard() {
    if (t_.joinable()) {
      t_.join();
    }
  }
  thread_guard(thread_guard const&) = delete;
  thread_guard& operator=(thread_guard const&) = delete;
private:
  std::thread& t_;
};

void do_something(int val) {
  std::cout << val << "\n";
}

void do_something_in_current_thread() {

}

struct func {
  int& i;
  func(int& i_) : i(i_) {}
  void operator() () {
    for (int j = 0; j < 5; ++ j) {
      do_something(i);
    }
  }
};

void oops() {
  int some_local_state{0};
  func my_func(some_local_state);
  std::thread my_thread(my_func);
  // thread_guard g(my_thread);
  my_thread.detach();
  assert(!my_thread.joinable());
  // 即使该函数发生异常导致oops()函数退出，按照析构顺序g中的线程也能够正常退出
  do_something_in_current_thread();
}
class Widget {

};
void update_data_for_widget(int w, Widget& data) {

}

void oops_again(int w) {
  Widget data;
  std::thread t(update_data_for_widget, w, data);
  // op
  t.join();
}

class X {
public:
  void do_lengthy_work() {
    std::cout << "do_lengthy_work\n";
  }
};

int main() {
  // oops();
  // oops_again(0);
  X my_x;
  // 该线程最终会调用my_x.do_lengthy_work
  std::thread t(&X::do_lengthy_work, &my_x);
  t.join();
  return 0;
}