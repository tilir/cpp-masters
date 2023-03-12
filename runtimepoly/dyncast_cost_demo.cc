//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Cost of different forms
//
//----------------------------------------------------------------------------

#include <iostream>

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

void *tomd(File *f) { return dynamic_cast<void *>(f); }

IOFile *tochild(File *f) { return dynamic_cast<IOFile *>(f); }

InputFile *tosibling(OutputFile *f) { return dynamic_cast<InputFile *>(f); }