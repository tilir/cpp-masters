//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  nth power: naive generalization
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

template <typename T> T nth_power(T x, unsigned n) {
  T acc = 1;
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

struct Matrix2x2 {
  int n[2][2];
  bool operator==(const Matrix2x2 &m) const {
    for (int i = 0; i < 2; ++i)
      for (int j = 0; j < 2; ++j)
        if (n[i][j] != m.n[i][j])
          return false;
    return true;
  }
  Matrix2x2 &operator*=(const Matrix2x2 &m) {
    Matrix2x2 r;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        int acc = 0;
        for (int k = 0; k < 2; k++)
          acc += n[i][k] * m.n[k][j];
        r.n[i][j] = acc;
      }
    }
    *this = r;
    return *this;
  }
};

} // namespace

TEST(functemplates, nthnaive) {
  EXPECT_EQ(nth_power(2, 11), (1 << 11));
  EXPECT_EQ(nth_power(2.0, 11), (1 << 11));
  Matrix2x2 m{1, 0, 0, 1};
  EXPECT_EQ(nth_power(m, 5), m);
}