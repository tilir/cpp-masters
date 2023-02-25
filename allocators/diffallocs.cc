//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Different allocators to motivate Halpern-like cases
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

constexpr int BUFSZ = 200;

template <typename T, size_t BufSize = BUFSZ>
using SmallVector = std::vector<T, short_alloc<T, BufSize, alignof(T)>>;

using SmallString = std::basic_string<char, std::char_traits<char>,
                                      short_alloc<char, BUFSZ, alignof(char)>>;

TEST(allocs, diffallocs) {
  SmallVector<SmallString>::allocator_type::arena_type a0;
  SmallString::allocator_type::arena_type a1, a2, a3;
  SmallVector<SmallString> v{a0};
  SmallString s1("hellofirststringgoodbyesso", a1);
  SmallString s2("hellosecondstringgoodbyesso", a2);
  SmallString s3("hellotmyhirdstringgoodbyesso", a3);
  std::cout << a0.used() << std::endl;
  std::cout << a1.used() << std::endl;
  std::cout << a2.used() << std::endl;
  std::cout << a3.used() << std::endl;

  v.push_back(s1);
  v.push_back(s2);
  v.reserve(4);

  std::cout << "---" << std::endl;
  std::cout << a0.used() << std::endl;
  std::cout << a1.used() << std::endl;
  std::cout << a2.used() << std::endl;
  std::cout << a3.used() << std::endl;

  v.insert(v.begin(), s3);
  std::cout << "---" << std::endl;
  std::cout << a0.used() << std::endl;
  std::cout << a1.used() << std::endl;
  std::cout << a2.used() << std::endl;
  std::cout << a3.used() << std::endl;

  EXPECT_EQ(v[0], s3);
  EXPECT_EQ(v[1], s1);
  EXPECT_EQ(v[2], s2);

  EXPECT_EQ(&v[0].get_allocator().get_arena(), &a1); // madness
  EXPECT_EQ(&v[1].get_allocator().get_arena(), &a2);
  EXPECT_EQ(&v[2].get_allocator().get_arena(), &a2);
}
