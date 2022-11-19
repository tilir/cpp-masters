//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// NRVO and RVO
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

int g = 1;

struct Verbose {
  Verbose() { g *= 2; }
  Verbose(const Verbose &) { g *= 3; }
  Verbose(Verbose &&) { g *= 5; }
  ~Verbose() { g *= 7; }
};

Verbose foo() { return Verbose(); }

Verbose bar() {
  Verbose foo;
  return foo;
}

TEST(rvrefs, nrvo) {
  { Verbose x = foo(); }
  EXPECT_EQ(g, 14); // we are pretty sure

  g = 1;
  { Verbose x = bar(); }
  EXPECT_EQ(g, 14); // hope so if NRVO and we are lucky
}