//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Explicit concept for lambda
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>
#include <type_traits>

namespace {

// clang-format off
template <typename T, typename U>
concept Addable = requires(T t, U u) {
  { t + u } -> std::convertible_to<std::common_type_t<T, U>>;
};
// clang-format on

} // namespace

TEST(lamdas, explicit_test) {
  // generic lambda with explicit types
  auto l = []<typename T, typename U>(T x, U y) requires Addable<T, U> {
    return x + y;
  };

  auto result = l(1, 1.0);
  EXPECT_EQ(result, 2);
}