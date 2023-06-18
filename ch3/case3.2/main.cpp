#include <iostream>
#include <mutex>
#include <string.h>

class some_data {
public:
  void do_something() {

  }
private:
  int a_;
  std::string b_;
};

class data_wrapper {
public:
  template<typename Function>
  void process_data(Function func) {
    std::lock_guard<std::mutex> l(m);
    func(data);
  }
private:
  some_data data;
  std::mutex m;
};

some_data* unprotected;

void malicious_function(some_data& protected_data) {
  unprotected = &protected_data;
}

data_wrapper x;

void foo() {
  // 这样做会破坏线程的安全性,将被保护的数据按照指针或者引用的方式
  // 传递给了没有收到保护的数据，使其脱离了lock_gurad的保护范围
  x.process_data(malicious_function);
  unprotected->do_something();
}

int main(int argc, char const *argv[])
{
  
  return 0;
}
