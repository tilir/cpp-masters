//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Example usage of PMR
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory_resource>

#include "gtest/gtest.h"

#include "memuse.hpp"

TEST(allocs, pmr) {
  auto initial = memuse();
  constexpr size_t sz = 1000 * sizeof(double);
  alignas(double) char buffer[sz];
  std::pmr::monotonic_buffer_resource alloc(buffer, sz);
  double start = 0.0;
  std::pmr::vector<double> v1(&alloc);
  std::generate_n(std::back_inserter(v1), 10,
                  [start]() mutable { return (start += 1.25); });
  EXPECT_EQ(memuse(), initial);

  for (int i = 0; i < v1.size(); ++i)
    EXPECT_EQ(v1[i], (i + 1) * 1.25);
}