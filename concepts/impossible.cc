//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// We can't do different cool things
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

namespace {

// no recursive definitions

template <bool b, bool... bs>
concept AllTrueRec = b &&((sizeof...(bs) == 0) ? true : AllTrueRec<bs...>);

template <bool b1, bool b2, bool b3>
requires AllTrueRec<b1, b2, b3>
int foo() { return 1; }

// no associated constraints to concepts

template <typename T>
concept Inner = requires {
  typename T::inner;
};

template <typename T>
requires Inner<T>
concept Outer = requires { typename T::outer; };

} // namespace

TEST(concepts, whatcanido) {
  auto x = foo<true, true, true>(); // corr
  EXPECT_EQ(x, 1);
}