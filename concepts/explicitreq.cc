//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Explicit requires over function. Looks somewhat odd.
//
//-----------------------------------------------------------------------------
#include "gtest/gtest.h"
#include <concepts>

// clang-format off
template <typename T, typename U> requires requires(T t, U u) {
  { t == u } -> std::convertible_to<bool>;
  { t != u } -> std::convertible_to<bool>;
  { u == t } -> std::convertible_to<bool>;
  { u != t } -> std::convertible_to<bool>;
}
// clang-format on
bool foo(T x, U y) {
  if ((x == y) && (y != x)) {
    // weak
    return false;
  }
  // strong
  return true;
}

struct W {};

bool operator==(W, int) { return true; }
bool operator==(int, W) { return true; }
bool operator!=(W, int) { return true; }
bool operator!=(int, W) { return true; }

TEST(concepts, explicitreq) {
  auto c1 = foo(1, W{}); // corr
  auto c2 = foo(1, 2);   // corr

  EXPECT_EQ(c1, false);
  EXPECT_EQ(c2, true);
}
