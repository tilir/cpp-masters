//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Virtual inheritance effect on initialization
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

struct InputFile : virtual public File {
  int b;
  InputFile(int b) : File(b * 2), b{b} {}
};

struct OutputFile : virtual public File {
  int c;
  OutputFile(int c) : File(c * 3), c{c} {}
};

struct IOFile : public InputFile, public OutputFile {
  int d;
  IOFile(int d) : File(d), InputFile(d * 5), OutputFile(d * 7), d{d} {}
};

#ifdef BUG
struct IOFile2 : public IOFile {
  int e;
  IOFile2(int e) : IOFile(e), e(e) {}
};
#endif

} // namespace

TEST(rtpoly, init_seq) {
  InputFile g(11);
  OutputFile h(11);
  EXPECT_EQ(g.a, 22);
  EXPECT_EQ(h.a, 33);

  IOFile f(11);
  EXPECT_EQ(f.InputFile::a, 11);
  EXPECT_EQ(f.OutputFile::a, 11);
  EXPECT_EQ(f.a, 11);
}