#include <mutex>
#include <iostream>
#include <thread>

int main() {
  std::mutex m1;
  std::mutex m2;
  {
    std::lock(m1, m2);
    std::lock_guard<std::mutex> lg1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> lg2(m2, std::adopt_lock);
  }

  std::cout << "run here\n";
  return 0;
}