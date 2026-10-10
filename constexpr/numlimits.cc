//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// simplest numerical limits example (not working)
//
//-----------------------------------------------------------------------------

#include <climits>
#include <concepts>
#include "gtest/gtest.h"

template <typename T>
struct my_numeric_limits;
template <>
struct my_numeric_limits<char> {
  static const size_t max() { return CHAR_MAX; }
};

int arr[my_numeric_limits<char>::max()]; // FAIL