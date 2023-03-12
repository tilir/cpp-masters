//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Do you understand why this test will fail?
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <type_traits>

namespace {

struct Capture {
  template <typename T, typename U> static auto func(T x, U y) {
    return x + y;
  };

  template <typename T, typename U>
  using func_t = std::decay_t<decltype(func<T, U>)>;

  template <typename T, typename U> operator func_t<T, U>() { return func; }
};

} // namespace

TEST(lamdas, generic_fail_demo) {
  Capture c;
  auto res = c(1, 1.0);
  EXPECT_EQ(res, 2);
}