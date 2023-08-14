//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Equality comparable SFINAE trait
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

} // namespace

TEST(concepts, eq) {
  constexpr bool string_int = is_equality_comparable<std::string, int>::value;
  constexpr bool long_char = is_equality_comparable<long &, char>::value;
  EXPECT_EQ(string_int, false);
  EXPECT_EQ(long_char, true);
}