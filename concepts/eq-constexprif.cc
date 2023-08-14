//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// if-constexpr approach. Works but intrusive.
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

namespace {

template <typename T, typename U, typename = void>
struct is_equality_comparable : std::false_type {};
template <typename T, typename U>
struct is_equality_comparable<
    T, U, std::void_t<decltype(std::declval<T>() == std::declval<U>())>>
    : std::true_type {};

template <typename T, typename U> bool check_eq(T &&lhs, U &&rhs) {
  if constexpr (!is_equality_comparable<T, U>::value) {
    static_assert(0 && "equality comparable expected");
    return false;
  } else {
    return (lhs == rhs);
  }
}

} // namespace

TEST(concepts, eqstatic) {
  constexpr bool string_int = is_equality_comparable<std::string, int>::value;
  constexpr bool long_char = is_equality_comparable<long &, char>::value;
  EXPECT_EQ(string_int, false);
  EXPECT_EQ(long_char, true);

  auto is_eq = check_eq(std::string{"1"}, 1); // CT error but pretty one
}