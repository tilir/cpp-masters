//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple PMR test resource tests
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <memory_resource>
#include <random>
#include <string>
#include <vector>

#include "pmrtestres.hpp"
#include "gtest/gtest.h"

static auto mtgen() {
  static std::random_device rd;
  static std::mt19937 g(rd());
  return g;
}

TEST(allocs, memres) {
  test_resource talloc;

  // allocate smth to leak
  (void)talloc.allocate(10, 4);

  double start = 0.0;
  std::pmr::vector<double> v1(&talloc);
  std::generate_n(back_inserter(v1), 100,
                  [start]() mutable { return (start += 1.25); });

  std::pmr::vector<double> v2(&talloc);
  v2.assign(v1.rbegin(), v1.rend());

  EXPECT_EQ(v1[42], 53.75);
  EXPECT_EQ(v2[42], 72.5);
}
