//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// C++98 allocs: finished logging alloc
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

  template <typename U> logging_alloc(const logging_alloc<U> &) {
    printf("coercion ctor\n");
  }

  logging_alloc() { printf("default ctor\n"); }
  logging_alloc(const logging_alloc &) { printf("copy ctor\n"); }

#if __cplusplus < 201103L
  // these required only for C++98, but having it is good practice
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  // rebind will be deprecated in C++20, but required in C++98
  template <typename U> struct rebind { typedef logging_alloc<U> other; };

  // C++98 also requires construct and destroy
  void construct(pointer p, const T &t) {
    printf("construct\n");
    new (p) T(t);
  }

  void destroy(pointer p) {
    printf("destroy\n");
    p->~T();
  }

  // C++98 also explicitly requires max_size
  size_type max_size() const {
    printf("max_size\n");
    return numeric_limits<size_type>::max() / sizeof(T);
  }
#endif
};

template <typename T>
bool operator==(const logging_alloc<T> &, const logging_alloc<T> &) {
  return true;
}

template <typename T>
bool operator!=(const logging_alloc<T> &, const logging_alloc<T> &) {
  return false;
}

TEST(allocs, logging) {
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