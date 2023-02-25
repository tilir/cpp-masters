//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple PMR allocator-aware container example
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <memory_resource>
#include <random>
#include <string>
#include <vector>

#include "memuse.hpp"
#include "slist.hpp"
#include "gtest/gtest.h"

TEST(allocs, slist) {
  auto initial = memuse();
  constexpr size_t sz = 1000 * sizeof(int);
  alignas(int) char buffer[sz];

  std::pmr::monotonic_buffer_resource res(buffer, sz);

  slist<int> s(&res);
  s.push_back(10);
  s.push_back(20);
  s.push_front(30);
  s.push_front(40);

  slist<int> s1(s, s.get_allocator());
  slist<int> s2(std::move(s1));

  for (auto x : s2)
    printf("%d\n", x);

  EXPECT_EQ(initial, memuse());
}
