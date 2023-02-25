//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Freelist allocator example tests
//
//----------------------------------------------------------------------------

#include <cstdio>
#include <limits>
#include <list>
#include <memory>
#include <vector>

#include "freelist.hpp"
#include "gtest/gtest.h"

TEST(allocs, freelist) {
  std::vector<int, freelist_alloc<int>> v;
  for (int i = 0; i < 16; ++i)
    v.push_back(i);
  printf("---\n");
  std::list<int, freelist_alloc<int>> l(v.begin(), v.end());
  printf("---\n");
  l.remove(2);
  l.remove(6);
  l.insert(l.begin(), -1);
  l.insert(l.begin(), -3);
  l.insert(l.begin(), -5);
  printf("---\n");
}