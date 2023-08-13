//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// has foobar: systematic SFINAE
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>
#include <type_traits>

namespace {

template <typename, typename = void>
struct has_typedef_foobar : std::false_type {};

template <typename T>
struct has_typedef_foobar<T, std::void_t<typename T::foobar>> : std::true_type {
};

struct foo {
  using foobar = float;
};

struct bar {};

} // namespace

TEST(sfinae, hasfoobar) {
  EXPECT_EQ(has_typedef_foobar<foo>::value, true);
  EXPECT_EQ(has_typedef_foobar<bar>::value, false);
}