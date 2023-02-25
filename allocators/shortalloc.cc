//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Hinnant's short alloc tests
//
//----------------------------------------------------------------------------

#include <cstdio>
#include <limits>
#include <list>
#include <memory>
#include <vector>

#include "memuse.hpp"
#include "shortalloc.hpp"
#include "gtest/gtest.h"

constexpr int VSZ = 10;
constexpr int BUFSZ = 200;

template <class T, size_t BufSize = BUFSZ>
using SmallVector = std::vector<T, short_alloc<T, BufSize, alignof(T)>>;

TEST(allocs, shortalloc) {
  // we want to check we are allocating everything from arena,
  // not from new. So storing initial to current memory usage.
  auto initial = memuse();
  SmallVector<int>::allocator_type::arena_type a;
  SmallVector<int> v{a};

  for (int i = 0; i < VSZ; ++i) {
    v.push_back(i);
    EXPECT_EQ(memuse(), initial);
  }

  // really we expect much more, since we are not freeing.
  // like for VSZ = 10 we expect: 1 + 2 + 4 + 8 + 16 ints
  // i.e. 31 * 4 = 124 out of 200
  EXPECT_EQ(a.used() >= VSZ * sizeof(int), true);

  // Yes, the correct values are actually in the vector
  for (int i = 0; i < VSZ; ++i)
    EXPECT_EQ(v[i], i);
}