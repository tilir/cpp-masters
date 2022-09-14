//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demo of instantiations, max function
//
//----------------------------------------------------------------------------

#pragma once

template <typename T>[[gnu::noinline]] T max(T x, T y) {
  return (x > y) ? x : y;
}

extern int foo(int x, int y);
extern int bar(int x, int y);