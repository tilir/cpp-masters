//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// constexpr new example
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <string>
#include <vector>

namespace {

constexpr int test() {
  int *p = new int(2);
  int k = *p;
  delete p;
  return k;
}

constexpr int testv() {
  std::vector<int> v = {1, 2, 3};
  return v[1];
}

constexpr int tests() {
  std::string s = "Hello";
  return s[1];
}

} // namespace

TEST(cexpr, opnew) {
  constexpr int x = test();
  EXPECT_EQ(x, 2);

  constexpr int y = testv();
  EXPECT_EQ(y, 2);

  constexpr int z = tests();
  EXPECT_EQ(z, 'e');
}