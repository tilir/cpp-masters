#include "gtest/gtest.h"

class Complex {
  double re, im;

public:
  constexpr Complex(double r = 0.0, double i = 0.0) : re(r), im(i) {}
  constexpr double real() const { return re; }
  constexpr double imag() const { return im; }
  constexpr Complex &operator+=(Complex rhs) {
    re += rhs.re;
    im += rhs.im;
    return *this;
  }
};

constexpr Complex operator+(Complex lhs, Complex rhs) {
  lhs += rhs;
  return lhs;
}

constexpr Complex operator"" _i(long double arg) { return Complex(0.0, arg); }

TEST(cexpr, complex) {
  constexpr Complex c = 0.0 + 1.0_i;
  constexpr Complex d = 1.0 + 2.0_i;
  constexpr Complex e = c + d;
  EXPECT_EQ(c.real(), 0.0);
  EXPECT_EQ(e.imag(), 3.0);
}