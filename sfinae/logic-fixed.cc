//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Fixed SFINAE logic: unwrap types
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

// instead std::integral_constant
template <typename T, T v> struct integral_constant {
  static const T value = v;
  typedef T value_type;
  typedef integral_constant type;
  operator value_type() const { return value; }
};

// instead std::true_type / std::false_type
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// instead std::is_same
template <typename T, typename U> struct is_same : false_type {};
template <typename T> struct is_same<T, T> : true_type {};
template <typename T, typename U>
using is_same_t = typename is_same<T, U>::type;
template <typename T, typename U>
bool is_same_v = is_same<T, U>::value; // не is_same_t<....>::value!

// some SFINAE logic
template <typename T, typename U> struct and_ : false_type {};
template <> struct and_<true_type, true_type> : true_type {};
template <typename T, typename U> bool and_v = and_<T, U>::value;

template <typename T> struct not_ : false_type {};
template <> struct not_<false_type> : true_type {};
template <typename T> using not_t = typename not_<T>::type;

} // namespace

TEST(sfinae, notfixed) {
  bool test1 = is_same_v<int, int> && !is_same_v<char, int>;
  bool test2 = and_v<is_same_t<int, int>, not_t<is_same_t<char, int>>>;
  EXPECT_EQ(test1, true); // OK
  EXPECT_EQ(test2, true); // OK
}