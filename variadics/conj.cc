//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// conjunction example for are_same_v
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <type_traits>

template <typename T, typename... Ts>
constexpr inline bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

template <typename... Ts>
auto sum_all(Ts &&...args) requires are_same_v<Ts...> {
  return (... + std::forward<Ts>(args));
}

TEST(variadic, conj) {
  int res;
  res = sum_all(1, 2, 3, 4);
  EXPECT_EQ(res, 10);
}