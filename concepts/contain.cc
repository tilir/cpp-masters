//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Unrestricted contains: ugly compilation bug
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

namespace {

template <typename R, typename T>
bool contains(R const &range, T const &value) {
  for (auto const &x : range)
    if (x == value)
      return true;
  return false;
}

} // namespace

TEST(concepts, contain) {
  std::vector<std::string> v{"0", "1", "2"};
  bool is_in = contains(v, 1);
  EXPECT_EQ(is_in, false);
}