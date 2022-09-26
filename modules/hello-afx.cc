//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// hello.cc -- investigate additional cost of headers
//
// try:
// > g++ -O2 stdafx.h
// > g++ -O2 -save-temps -S hello-afx.cc
//
// note you need the same parameters:
// > g++ -save-temps -S hello-afx.cc
//
//------------------------------------------------------------------------------

#include "stdafx.h"

int main() {
  std::cout << "Hello" << std::endl;
}
