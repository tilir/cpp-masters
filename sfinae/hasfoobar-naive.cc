//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// has foobar: ad-hoc SFINAE
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

namespace {

template <typename T> struct has_typedef_foobar {
  using yes = char[1];
  using no = char[2];
  template <typename C> static yes &test(typename C::foobar *);
  template <typename> static no &test(...);
  enum { value = (sizeof(test<T>(0)) == sizeof(yes)) };
};

struct foo {
  using foobar = float;
};

struct bar {};

} // namespace

TEST(sfinae, hasfoobarnaive) {
  EXPECT_EQ(has_typedef_foobar<foo>::value, true);
  EXPECT_EQ(has_typedef_foobar<bar>::value, false);
}