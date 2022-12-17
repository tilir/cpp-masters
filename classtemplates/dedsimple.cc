//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Deduction guide without ctor
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <string>

template <typename T> struct NamedValue {
  T value;
  std::string name;
};

NamedValue(const char *, const char *)->NamedValue<std::string>;

TEST(classtempls, dedsimple) {
  NamedValue n{"hello", "world"}; // OK
  EXPECT_EQ(n.name, "world");
}