//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Even more interesting UB case demo:
// try with std equals C++17 and C++20
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

constexpr int djb2(char const *str) {
  int hash = 5381;
  int c = 0;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

  return hash;
}

TEST(sfinae, moreub) {
  constexpr int x = djb2("hello you :)");
  EXPECT_EQ(x, -466252103);
}