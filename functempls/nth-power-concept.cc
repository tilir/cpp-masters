//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  nth power: adding concepts
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

template <typename T>
concept multiplicative = requires(T t) {
  { t *= t } -> std::convertible_to<T>;
};

template <typename T>
T do_nth_power(T x, T acc,
               unsigned n) requires multiplicative<T> && std::copyable<T> {
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

unsigned nth_power(unsigned x, unsigned n) {
  if (x < 2u || n == 1u)
    return x;
  return do_nth_power<unsigned>(x, 1u, n);
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

Matrix2x2 nth_power(Matrix2x2 x, unsigned n) {
  Matrix2x2 eye{1, 0, 0, 1};
  if (x == eye || n == 1u)
    return x;
  return do_nth_power<Matrix2x2>(x, eye, n);
}

struct Wrong {};

Wrong nth_power(Wrong x, unsigned n) {
  Wrong eye;
  if (n == 1u)
    return x;
  return do_nth_power<Wrong>(x, eye, n);
}

TEST(nthpower, concepttest) {
  EXPECT_EQ(nth_power(2, 11), (1 << 11));
  EXPECT_EQ(nth_power(2.0, 11), (1 << 11));
  Matrix2x2 m{1, 0, 0, 1};
  EXPECT_EQ(nth_power(m, 5), m);
}