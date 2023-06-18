#include "hierarchical_mutex.h"
#include <thread>

hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);
hierarchical_mutex other_mutex(6000);
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value = 0;
int do_low_level_stuff() {
  std::cout << "do_low_level_stuff\n";
  return 0;
}

int low_level_func() {
  std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
  return do_low_level_stuff();
}

void high_level_stuff(int some_param) {
  std::cout << "high_level_stuff\n";
}

void high_level_func() {
  std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
  high_level_stuff(low_level_func());
}

void thread_a() {
  high_level_func();
}

void do_other_stuff() {
  std::cout << "do_other_stuff\n";
}

void other_stuff() {
  high_level_func();
  do_other_stuff();
}

void thread_b() {
  std::lock_guard<hierarchical_mutex> lk(other_mutex);
  other_stuff();
}

int main() {
  std::thread ta(thread_a);
  std::thread tb(thread_b);

  ta.join();
  tb.join();
  return 0;
}