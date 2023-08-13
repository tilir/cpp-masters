//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Complex numbers example: literal operands
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

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

} // namespace

TEST(cexpr, complexop) {
  constexpr Complex c = 0.0 + 1.0_i;
  constexpr Complex d = 1.0 + 2.0_i;
  constexpr Complex e = c + d;
  EXPECT_EQ(c.real(), 0.0);
  EXPECT_EQ(e.imag(), 3.0);
}