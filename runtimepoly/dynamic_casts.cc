//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Different forms of dynamic_cast
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

} // namespace

TEST(rtpoly, dynamic_casts) {
  File *onlyin = new InputFile(11);
  File *iof = new IOFile(11);
  std::cout << "iof: " << iof << std::endl;

  IOFile *md = dynamic_cast<IOFile *>(iof);
  std::cout << "IOFile: " << md << std::endl;

  InputFile *inf = dynamic_cast<InputFile *>(iof);
  std::cout << "inf: " << inf << std::endl;

  OutputFile *outf = dynamic_cast<OutputFile *>(inf);
  EXPECT_NE(outf, nullptr);
  std::cout << "outf: " << outf << std::endl;

  outf = dynamic_cast<OutputFile *>(onlyin);
  EXPECT_EQ(outf, nullptr);

  void *voif = dynamic_cast<void *>(iof);
  EXPECT_NE(voif, nullptr);
  std::cout << "voif: " << voif << std::endl;
}