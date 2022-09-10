//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  nth power: concrete solution
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

unsigned nth_power(unsigned x, unsigned n) {
  unsigned acc = 1;
  if ((x < 2) || (n == 1))
    return x;
  while (n > 0) {
    if ((n & 0x1) == 0x1) {
      acc *= x;
      n -= 1;
    }
    x *= x;
    n /= 2;
  }
  return acc;
}

unsigned naive_nth_power(unsigned x, unsigned n) {
  unsigned acc = 1;
  if ((x < 2) || (n == 1))
    return x;
  for (unsigned k = 0; k < n; ++k)
    acc *= x;
  return acc;
}

TEST(nthpower, concrete) {
  EXPECT_EQ(nth_power(0, 10), 0);
  EXPECT_EQ(nth_power(1, 11), 1);
  EXPECT_EQ(nth_power(2, 11), (1 << 11));
  EXPECT_EQ(nth_power(3, 0), 1);
  EXPECT_EQ(nth_power(3, 3), 27);
  EXPECT_EQ(nth_power(3, 15), 14348907);
  for (int x = 0; x < 10; ++x)
    for (int p = 0; p < 10; ++p)
      EXPECT_EQ(nth_power(x, p), naive_nth_power(x, p));
}