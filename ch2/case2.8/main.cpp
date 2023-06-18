#include "parallel_accumulate.h"

int main() {
  const int N = 10000000;
  std::vector<int> v(1000000000);
  for (long long i = 0; i < N; ++ i) {
    v[i] = i;
  }
  unsigned long long ans = parallel_accumulate(v.begin(), v.end(), 0);
  printf("ans = %llu\n", ans);
  return 0;
}