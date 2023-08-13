//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// numerical limits example fixed
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <climits>
#include <concepts>

namespace {

template <typename T> struct my_numeric_limits;
template <> struct my_numeric_limits<char> {
  static constexpr size_t max() { return CHAR_MAX; }
};

int arr[my_numeric_limits<char>::max()]; // OK

} // namespace

TEST(cexpr, ctarr) {
  auto n = sizeof(arr);
  EXPECT_EQ(n, CHAR_MAX * sizeof(int));
}