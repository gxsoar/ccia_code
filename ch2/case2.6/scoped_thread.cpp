#include <iostream>
#include <thread>

class scoped_thread {
public:
  explicit scoped_thread(std::thread t) : t_(std::move(t)) {
    // 线程创建失败异常
    if (!t_.joinable()) {
      throw std::logic_error("No thread");
    }
  }
  ~scoped_thread() {
    t_.join();
  }
  scoped_thread(scoped_thread const &)=delete;
  scoped_thread& operator=(scoped_thread const &)=delete;
private:
  std::thread t_;
};

void do_something(int val) { std::cout << val << "\n"; }

struct func {
  int& i;
  func(int& i_) : i(i_) {}
  void operator() () {
    for (int j = 0; j < 3; ++ j) {
      do_something(i);
    }
  }
};

void f() {
  int some_local_state = 0;
  // 通过scoped_thread单独创建一个线程，等到f()运行结束的时候在
  // 执行析构函数，然后再t再join
  scoped_thread t{std::thread(func(some_local_state))};
}

int main(int argc, char const *argv[])
{
  f();
  return 0;
}
