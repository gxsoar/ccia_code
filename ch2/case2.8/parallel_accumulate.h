// 并行版的accumulate
#include <iostream>
#include <algorithm>
#include <thread>
#include <numeric>

template<typename Iterator, typename T>
struct accumulate_block {
  void operator() (Iterator first, Iterator last, T& result) {
    result = std::accumulate(first, last, result);
  }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
  unsigned long long const length = std::distance(first, last);
  if (!length) return init;
  unsigned long long const min_per_thread = 25;
  // 计算最多需要开启多少个线程, 再每个线程至少处理25块的数据的情况下
  unsigned long long const max_threads = (length + min_per_thread - 1) / min_per_thread;
  unsigned long long const hareware_threads = std::thread::hardware_concurrency();
  unsigned long long const num_threads = std::min(hareware_threads != 0 ? hareware_threads : 2, max_threads);
  unsigned long long block_size = length / num_threads;
  std::vector<T> results(num_threads);
  std::vector<std::thread> threads(num_threads - 1);  // -1 是因为还有一个主线程
  Iterator block_start = first;
  for (unsigned long long i = 0; i < (num_threads - 1); ++ i) {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    // 在这个地方用std::ref(results[i])应为，再线程中会将resultes[i]进行一次拷贝，然后再将其结果作为一个右值传递给
    // 函数的形参
    threads[i] = std::thread(
      accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i])
    );
    block_start = block_end;
  }
  accumulate_block<Iterator, T>() (
    block_start, last, results[num_threads - 1] 
  );
  for (auto &e : threads) {
    e.join();
  }
  // 最后再有所有的主线程计算,汇总所有的结果
  return std::accumulate(results.begin(), results.end(), init);
}