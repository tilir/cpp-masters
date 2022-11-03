//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Trit example with trit literals
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename T = int> constexpr T ct_trit(const char *t) {
  T x = 0;
  size_t b = 0;
  for (size_t i = 0; t[i] != '\0'; ++i) {
    switch (t[i]) {
    case '\'':
      break;
    case '0':
      x = (x * 3);
      ++b;
      break;
    case '1':
      x = (x * 3) + 1;
      ++b;
      break;
    case 'j':
      x = (x * 3) - 1;
      ++b;
      break;
    default:
      throw "Only '0', '1', and ',' may be used";
    }
  }
  return x;
}

constexpr long long operator"" _trit(char const *s, size_t len) {
  return ct_trit<long long>(s);
}

TEST(cexpr, complex) {
  constexpr unsigned long long c = "10j01"_trit;
  EXPECT_EQ(c, 73);
}