//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Scoped allocator adapter usage. Note: example is problematic.
//
// see working boost::interprocess example here:
// https://en.cppreference.com/w/cpp/memory/scoped_allocator_adaptor
//
//----------------------------------------------------------------------------

#include <cstdio>
#include <limits>
#include <list>
#include <memory>
#include <scoped_allocator>
#include <vector>

#include "memuse.hpp"
#include "shortalloc.hpp"
#include "gtest/gtest.h"

constexpr int BUFSZ = 256; // must be multiple of align

using SmallString = std::basic_string<char, std::char_traits<char>,
                                      short_alloc<char, BUFSZ, alignof(char)>>;

template <typename T, size_t BufSize = BUFSZ>
using SmallVector = std::vector<T, short_alloc<T, BufSize, alignof(T)>>;

template <typename T>
using ScopedSmallVector = std::vector<
    T, std::scoped_allocator_adaptor<short_alloc<T, BUFSZ, alignof(T)>>>;

TEST(allocs, scopedallocs) {
  // non-scoped example
#if 0
  SmallVector<SmallString>::allocator_type::arena_type a0;  
  SmallVector<SmallString> v{a0};
  v.push_back("hellofirststringgoodbyesso"); // <--- error here, allocator unknown
  v.push_back("hellosecondstringgoodbyesso");
  v.reserve(4);
  std::cout << a0.used() << std::endl;
  v.insert(v.begin(), "hellotmyhirdstringgoodbyesso");
  std::cout << "---" << std::endl;
  std::cout << a0.used() << std::endl;
#endif

  // scoped example shall work but it don't. Why?
  ScopedSmallVector<SmallString>::allocator_type::arena_type sa0;
  ScopedSmallVector<SmallString> sv{sa0};
  sv.emplace_back(
      "hellofirststringgoodbyesso"); // <--- shall be ok by adaptor magic
  sv.emplace_back("hellosecondstringgoodbyesso");
  sv.reserve(4);
  std::cout << sa0.used() << std::endl;
  sv.emplace(sv.begin(), "hellotmyhirdstringgoodbyesso");
  std::cout << "---" << std::endl;
  std::cout << sa0.used() << std::endl;
}
