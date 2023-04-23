//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// pack syntax in requires expression
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <type_traits>

template <typename T, typename... Ts>
auto sum_all(T &&arg,
             Ts &&...args) requires(std::is_same_v<T, Ts> &&... && true) {
  return std::forward<T>(arg) + (... + std::forward<Ts>(args));
}

TEST(variadic, reqpack) {
  int res;
  res = sum_all(1, 2, 3, 4);
  EXPECT_EQ(res, 10);
}