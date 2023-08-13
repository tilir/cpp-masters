//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// CE binary number parser
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

template <typename... Ts>
constexpr unsigned long long binparser(unsigned long long accum, char c,
                                       Ts... cs) {
  unsigned digit = (c == '1') ? 1 : (c == '0') ? 0 : throw "out of range";
  if constexpr (sizeof...(Ts) != 0)
    return binparser(accum * 2 + digit, cs...);
  return accum * 2 + digit;
}

template <char... Chars> constexpr unsigned long long operator"" _binary() {
  return binparser(0ull, Chars...);
}

} // namespace

TEST(cexpr, binparser) {
  constexpr unsigned long long c = 1010101010101_binary;
  EXPECT_EQ(c, 5461);
}