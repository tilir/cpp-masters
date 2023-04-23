//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Parallel algorithm test
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <execution>
#include <iterator>
#include <numeric>
#include <vector>

#include "gtest/gtest.h"

namespace exec = std::execution;

TEST(parallel, policy) {
  bool Res;
  std::vector<int> V1, V2, V3;
  V1.reserve(1000);
  std::iota(V1.begin(), V1.end(), 0);
  std::copy(exec::seq, V1.begin(), V1.end(), std::back_inserter(V2));
  std::copy(exec::par, V1.begin(), V1.end(), std::back_inserter(V3));
  Res = std::equal(V2.begin(), V2.end(), V3.begin());
  EXPECT_EQ(Res, true);
  Res = std::equal(exec::seq, V2.begin(), V2.end(), V3.begin());
  EXPECT_EQ(Res, true);
  Res = std::equal(exec::par, V2.begin(), V2.end(), V3.begin());
  EXPECT_EQ(Res, true);
  Res = std::equal(exec::par_unseq, V2.begin(), V2.end(), V3.begin());
  EXPECT_EQ(Res, true);
}
