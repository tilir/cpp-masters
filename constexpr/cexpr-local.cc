//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Constexpr local variable fallacy case
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <initializer_list>

template <typename T> consteval auto ilist_sz(std::initializer_list<T> init) {
#if defined(BAD)
  constexpr auto init_sz = init.size();
#else
  auto init_sz = init.size();
#endif
  return init_sz;
}

TEST(cexpr, cevinit) {
  constexpr auto n = ilist_sz({1, 2, 3});
  EXPECT_EQ(n, 3);
}