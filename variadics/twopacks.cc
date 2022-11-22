//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Expansion of two packs at once
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <utility>

template <typename I, I... is1, I... is2>
constexpr auto concat(std::integer_sequence<I, is1...>,
                      std::integer_sequence<I, is2...>) {
  return std::integer_sequence<I, is1..., is2...>{};
}

template <typename I, I i1, I... is1>
constexpr auto sum(std::integer_sequence<I, i1, is1...>) {
  if constexpr (sizeof...(is1) != 0)
    return i1 + sum(std::integer_sequence<I, is1...>{});
  return i1;
}

TEST(variadic, twopacks) {
  std::integer_sequence<int, 1, 2, 3> i1;
  std::integer_sequence<int, 4, 5, 6> i2;
  auto i3 = concat(i1, i2);
  auto res = sum(i3);
  EXPECT_EQ(res, 21);
}