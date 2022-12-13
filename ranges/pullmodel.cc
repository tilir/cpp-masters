//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// pull model demo
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;
namespace views = std::views;

TEST(rngs, pullmodel) {
  std::vector v = {0, 1, 2, 3, 4, 5, 6};

  auto is_even = [](auto i) {
    std::cout << "even: " << i << std::endl;
    return ((i % 2) == 0);
  };

  auto square = [](auto i) {
    std::cout << "square: " << i << std::endl;
    return i * i;
  };

  std::cout << "--- First ---" << std::endl;
  for (auto i :
       v | ranges::views::filter(is_even) | ranges::views::transform(square))
    std::cout << "number: " << i << std::endl;

  std::cout << "--- Second ---" << std::endl;
  for (auto i :
       v | ranges::views::transform(square) | ranges::views::filter(is_even))
    std::cout << "number: " << i << std::endl;
}