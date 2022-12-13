//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// ranges::dangling example
//
//----------------------------------------------------------------------------

#include "boost/type_index.hpp"
#include "gtest/gtest.h"
#include <numeric>
#include <ranges>
#include <vector>

namespace ti = boost::typeindex;
namespace ranges = std::ranges;

std::vector<int> foo(int n) {
  std::vector<int> ret(n);
  std::iota(ret.begin(), ret.end(), 0);
  return ret;
}

TEST(rngs, nondangled) {
  auto v = foo(42);
  auto res1 = ranges::find(v, 21);
  std::cout << ti::type_id_with_cvr<decltype(res1)>().pretty_name()
            << std::endl;
  auto is_dangle = std::is_same_v<decltype(res1), std::ranges::dangling>;
  EXPECT_EQ(is_dangle, false);
  EXPECT_EQ(*res1, 21);
}

TEST(rngs, dangled) {
  auto res1 = ranges::find(foo(42), 21);
  std::cout << ti::type_id_with_cvr<decltype(res1)>().pretty_name()
            << std::endl;
  auto is_dangle = std::is_same_v<decltype(res1), std::ranges::dangling>;
  EXPECT_EQ(is_dangle, true);

#if 0
  // no operator * in std::ranges::dangling
  EXPECT_EQ(*res1, 21);
#endif
}