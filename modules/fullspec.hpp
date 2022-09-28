//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// inline specialization example
//
// try:
// > g++ fullspec-1.cc fullspec-2.cc
// > g++ fullspec-1.cc fullspec-2.cc -DSP
// > g++ fullspec-1.cc fullspec-2.cc -DSPI
//
//-----------------------------------------------------------------------------

#pragma once

int foo(int);

template <typename T> T maxcollatz(T n) {
  T cmax, xmax;
  for (T x = 2; x < n; ++x) {
    T y = x;
    T c = 0;
    while (y > 1) {
      c += 1;
      if ((y % 2) == 0)
        y = y / 2;
      else
        y = 3 * y + 1;
    }
    if (c > cmax) {
      cmax = c;
      xmax = x;
    }
  }
  return xmax;
}

#if defined(SP)

template <> int maxcollatz(int n) {
  int cmax, xmax;
  for (int x = 2; x < n; ++x) {
    int y = x;
    int c = 0;
    while (y > 1) {
      c += 1;
      if ((y % 2) == 0)
        y = y / 2;
      else
        y = 3 * y + 1;
    }
    if (c > cmax) {
      cmax = c;
      xmax = x;
    }
  }
  return xmax;
}

#endif

#if defined(SPI)

template <> inline int maxcollatz(int n) {
  int cmax, xmax;
  for (int x = 2; x < n; ++x) {
    int y = x;
    int c = 0;
    while (y > 1) {
      c += 1;
      if ((y % 2) == 0)
        y = y / 2;
      else
        y = 3 * y + 1;
    }
    if (c > cmax) {
      cmax = c;
      xmax = x;
    }
  }
  return xmax;
}

#endif