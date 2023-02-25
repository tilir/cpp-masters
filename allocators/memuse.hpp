//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Memory usage: redefinition of operator new to track standard allocations
//
//----------------------------------------------------------------------------

#pragma once

#include <tuple>

inline size_t memory = 0;
inline size_t alloc = 0;

inline void *operator new(size_t s) noexcept(false) {
  memory += s;
  ++alloc;
  return malloc(s);
}

inline void operator delete(void *p) throw() {
  --alloc;
  free(p);
}

inline auto memuse() { return std::make_pair(memory, alloc); }
