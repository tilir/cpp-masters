//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// C++98 allocs: first attempt
//
//----------------------------------------------------------------------------

#include <cstdio>
#include <limits>
#include <list>
#include <memory>
#include <vector>

#include "gtest/gtest.h"

template <typename T> struct logging_alloc {
  typedef T value_type;
  typedef T *pointer;

  pointer allocate(size_t n) {
    printf("allocate %zu elements\n", n);
    void *Chunk = ::operator new(n * sizeof(value_type));
    return static_cast<pointer>(Chunk);
  }

  void deallocate(pointer p, size_t n) {
    printf("deallocate\n");
    ::operator delete(p);
  }

  logging_alloc() { printf("default ctor\n"); }
  logging_alloc(const logging_alloc &) { printf("copy ctor\n"); }
};

TEST(allocs, attempt1) {
  std::vector<int, logging_alloc<int>> v;

  for (int i = 0; i < 16; ++i)
    v.push_back(i);
  printf("---\n");
  std::vector<int, logging_alloc<int>> v2 = v;
  v2.push_back(16);
  EXPECT_EQ(v2[16], 16);

// problems:

// 1. Assign (what to do with allocator on assign?)
#if defined(PROBLEM1)
  v = v2;
#endif

// 2. Rebind (how to create allocator for the list node from T?)
#if defined(PROBLEM2)
  std::list<int, logging_alloc<int>> l(v.begin(), v.end());
#endif
}
