#include "joining_thread.h"

int main(int argc, char const *argv[])
{
  {
    auto func = []() { std::cout << "func\n"; };

    joining_thread t(func);
  }
  return 0;
}

