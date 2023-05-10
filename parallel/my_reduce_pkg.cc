//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Self-written reduce with packaged tasks
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <execution>
#include <future>
#include <iterator>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

namespace exec = std::execution;

namespace {

constexpr int MINLEN = 25;

unsigned determine_threads(unsigned length) {
  const unsigned long min_per_thread = MINLEN;
  unsigned long max_threads = length / min_per_thread;
  unsigned long hardware_conc = std::thread::hardware_concurrency();
  return std::min(hardware_conc != 0 ? hardware_conc : 2, max_threads);
}

template <typename Iterator,
          typename T = typename std::iterator_traits<Iterator>::value_type>
T parallel_accumulate(Iterator first, Iterator last, T init = 0) {
  long length = distance(first, last);
  if (0 == length)
    return init;
  if (length <= MINLEN)
    return std::accumulate(first, last, init);

  static unsigned nthreads = determine_threads(length);
  long bsize = length / nthreads;

  std::vector<std::future<T>> results(nthreads);

  auto accumulate_block = [](Iterator first, Iterator last) {
    return std::accumulate(first, last, T{});
  };

  unsigned tidx = 0;

  for (; length >= bsize * (tidx + 1); first += bsize, tidx += 1) {
    std::packaged_task<T(Iterator, Iterator)> task{accumulate_block};
    results[tidx] = task.get_future();
    std::thread t{move(task), first, first + bsize};
    t.detach();
  }

  auto remainder = length - bsize * tidx;

  T result = init;

  if (remainder > 0)
    result += accumulate_block(first, first + remainder);

  for (unsigned long i = 0; i < nthreads; ++i)
    result += results[i].get();

  return result;
}

} // namespace

TEST(parallel, reduce_pkg) {
  constexpr int TSZ = 500;
  std::random_device RD;
  std::mt19937 MT(RD());
  std::vector<int> V(TSZ);
  std::iota(V.begin(), V.end(), 0);
  std::shuffle(V.begin(), V.end(), MT);
  auto res = parallel_accumulate(V.begin(), V.end(), 0);
  auto stand_res = std::accumulate(V.begin(), V.end(), 0);
  EXPECT_EQ(res, stand_res);
}