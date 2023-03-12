//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Lavavej's projection
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <iostream>
#include <vector>

namespace {

template <typename Range, typename Callable>
void print_range(Range &&r, Callable &&c) {
  for (auto &&e : std::forward<Range>(r))
    std::cout << std::invoke(std::forward<Callable>(c),
                             std::forward<decltype(e)>(e))
              << " ";
  std::cout << std::endl;
}

} // namespace

TEST(lamdas, projection) {
  std::vector<std::pair<int, int>> v = {{1, 1}, {2, 2}, {3, 3}};
  print_range(v, [](const auto &p) { return p.second; });
  print_range(v, &std::pair<int, int>::second);
}