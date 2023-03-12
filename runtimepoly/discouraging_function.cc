//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Return to PMR allocators example: what to do with std function?
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory_resource>

#include "memuse.hpp"
#include "gtest/gtest.h"

TEST(rtpoly, discouraging_function) {
  constexpr size_t sz = 1000 * sizeof(double);
  alignas(double) char buffer[sz];
  auto use1 = memuse();
  std::pmr::monotonic_buffer_resource alloc(buffer, sz);
  double start = 0.0;
  std::pmr::vector<double> v1(&alloc);
  std::function<void(void)> f = [v1, start]() mutable {
    std::generate_n(std::back_inserter(v1), 10,
                    [start]() mutable { return (start += 1.1); });
  };
  f();
  for (auto x : v1)
    std::cout << x << std::endl;
  auto use2 = memuse();
  EXPECT_NE(use1, use2);
}
