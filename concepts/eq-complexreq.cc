//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// check_eq constrained with requires expression
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <string>

namespace {

template <typename T, typename U>
requires requires(T t, U u) { t == u; }
bool check_eq(T &&lhs, U &&rhs) { return (lhs == rhs); }

} // namespace

TEST(concepts, reqreq) {
  bool a, b, c;
  a = check_eq(1, 2);
  EXPECT_EQ(a, false);
  b = check_eq(std::string{"1"}, std::string{"2"});
  EXPECT_EQ(b, false);
  c = check_eq(std::string{"1"}, 1);
  EXPECT_EQ(c, false);
}