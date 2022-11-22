//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// void-constructible comma discrimination
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

// #define BUG

struct S {};
constexpr int operator,(S, char) { return 14; }
#if defined(BUG)
constexpr int operator,(S, void) { return 42; }
#endif

constexpr auto f() { return S{}, 'c'; }
constexpr auto g() { return S{}, void(); }

static_assert(std::is_same_v<decltype(g()), void>, "g() is void");

TEST(variadic, commas) {
  constexpr int i = f();
#if defined(BUG)
  constexpr int j = g(); // error
#endif
  EXPECT_EQ(i, 14);
}