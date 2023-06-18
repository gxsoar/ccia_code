#include <thread>
#include <iostream>

void some_function() { printf("some_function\n"); }

void some_other_function() { printf("some_other_function\n"); }

int main(int argc, char const *argv[])
{
  std::thread t1(some_function);
  // 线程是只移型别，只能通过移动的方式来改变归属权
  std::thread t2 = std::move(t1);
  t1 = std::thread(some_other_function);  // t1来运行some_other_function
  std::thread t3;
  t3 = std::move(t2); // 将t2的归属权移交给t3
  // some_funtion的归属权又移交给t1,这个时候调用std::terminate终止该线程
  t1 = std::move(t3);
  return 0;
}
