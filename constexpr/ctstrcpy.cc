//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// compile-time strcpy
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <array>

constexpr char *strncopy(char *dst, const char *src, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    dst[i] = src[i];
    if (dst[i] == '\0')
      return &dst[i + 1];
  }
  return nullptr;
}

template <size_t N1, size_t N2>
constexpr auto concat(char const (&a)[N1], char const (&b)[N2]) {
  std::array<char, N1 + N2 - 1> result = {};
  char *next = strncopy(result.data(), a, N1 - 1);
  char *nextdst = next ? next : &result[N1 - 1];
  strncopy(nextdst, b, N2);
  result.back() = '\0';
  return result;
}

TEST(cexpr, array) {
  constexpr auto hello = concat("Hello, ", "world!");
  EXPECT_EQ(hello.back(), '\0');
  EXPECT_EQ(hello[7], 'w');
}