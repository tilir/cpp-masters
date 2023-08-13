//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Floating compile time constness
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

struct S1 {
  static const int sz = 256;
};
const int page_sz1 = 4 * S1::sz;
int arr1[page_sz1]; // ok, CT constant

struct S2 {
  static const int sz;
};
const int page_sz2 = 4 * S2::sz;
const int S2::sz = 256;
int arr2[page_sz2]; // error: not CT constant