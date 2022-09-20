//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Danger example: see also danger1
//
//----------------------------------------------------------------------------

#include <iostream>

template <typename T> struct D;

// we explicitly specialized D<void>
// but it was instanced in another TU
// this is clearly IFNDR
template <> struct D<void> {
  enum { max = 100 };
};

void clear(char *buf) {
  std::cout << "cleaning up " << D<void>::max << std::endl;
  for (int k = 0; k < D<void>::max; ++k)
    buf[k] = 0;
}
