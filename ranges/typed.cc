//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// different types for ranges
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>

namespace ranges = std::ranges;

#include "boost/type_index.hpp"
namespace ti = boost::typeindex;

struct Empty {
  int *begin() { return nullptr; }
  int *end() { return nullptr; }
};

int main() {
  std::string s = "Hello, ranges world";
  std::string_view svv(s);
  std::cout << ti::type_id_with_cvr<decltype(svv)>().pretty_name() << std::endl;
  auto sv = ranges::views::all(s);
  std::cout << ti::type_id_with_cvr<decltype(sv)>().pretty_name() << std::endl;
  auto sagain = sv.base();
  std::cout << ti::type_id_with_cvr<decltype(sagain)>().pretty_name()
            << std::endl;

  Empty e;
  auto ev = ranges::views::all(e);
}
