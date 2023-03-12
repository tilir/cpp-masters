//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Base visibility requirement
//
//----------------------------------------------------------------------------

#include <iostream>
#include <memory>
#include <sstream>

#include "gtest/gtest.h"

namespace {

struct File {
  int a;
  File(int a) : a{a} {}
  virtual ~File() {}
};

struct InputFile : protected File {
  int b;
  InputFile(int b) : File(b * 2), b{b} {}
  File *spawn() { return static_cast<File *>(this); }
};

} // namespace

TEST(rtpoly, visible_base) {
  InputFile *inf = new InputFile(11);
  File *f = inf->spawn(); // ok
#if (ERROR == 1)
  f = static_cast<File *>(inf); // fail
#endif
  delete f;
}