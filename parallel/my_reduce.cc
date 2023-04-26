//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple and naive self-written reduce
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <execution>
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

  std::vector<std::thread> threads(nthreads);
  std::vector<T> results(nthreads + 1);

  auto accumulate_block = [](Iterator first, Iterator last, T &result) {
    result = std::accumulate(first, last, result);
  };

  unsigned tidx = 0;

  for (; length >= bsize * (tidx + 1); first += bsize, tidx += 1)
    threads[tidx] = std::thread(accumulate_block, first, first + bsize,
                                std::ref(results[tidx]));

  auto remainder = length - bsize * tidx;

  if (remainder > 0) {
    assert(tidx == nthreads);
    accumulate_block(first, first + remainder, std::ref(results[tidx]));
  }

  for (auto &&t : threads)
    t.join();

  return std::accumulate(results.begin(), results.end(), init);
}

} // namespace

TEST(parallel, reduce) {
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