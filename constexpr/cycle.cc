//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Cycle elements
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <array>

namespace {

template <size_t N, typename T, size_t Size>
constexpr std::array<T, N * Size> cycle_elems(std::array<T, Size> a) {
  std::array<T, N * Size> result{};
  for (T i = 0; i < N * Size; ++i)
    result[i] = a[i % Size];
  return result;
}

} // namespace

TEST(cexpr, array) {
  constexpr std::array<size_t, 5> arr = {0, 4, 2, 1, 3};
  constexpr auto cycled = cycle_elems<3>(arr);
  EXPECT_EQ(cycled[1], 4);
  EXPECT_EQ(cycled[1], cycled[6]);
  EXPECT_EQ(cycled[7], 2);
  EXPECT_EQ(cycled[7], cycled[12]);
}