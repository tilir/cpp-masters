//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Dreaded diamond without virtual inheritance
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

struct InputFile : public File {
  int b;
  InputFile(int b) : File(b * 2), b{b} {}
};

struct OutputFile : public File {
  int c;
  OutputFile(int c) : File(c * 3), c{c} {}
};

struct IOFile : public InputFile, public OutputFile {
  int d;
  IOFile(int d) : InputFile(d * 5), OutputFile(d * 7), d{d} {}
};

} // namespace

TEST(rtpoly, romb_novirt) {
  IOFile *iof = new IOFile(11);
  InputFile *inf = iof;
  iof = static_cast<IOFile *>(inf);

#if (ERROR == 1)
  File *f = iof;
#endif
}