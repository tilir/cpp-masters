//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// change inside filter is broken
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;
namespace views = std::views;

int main() {
  std::vector v = {1, 4, 7, 10};
  auto vf = views::filter(v, [](int n) { return n % 2; });
  // vf : 4, 10

  for (int &i : vf)
    i += 1;
  // vf : ????

  ranges::copy(v, std::ostream_iterator<int>{std::cout, " "});
  std::cout << std::endl;

  for (int &i : vf)
    i += 1;
  // vf : ????

  ranges::copy(v, std::ostream_iterator<int>{std::cout, " "});
  std::cout << std::endl;
}