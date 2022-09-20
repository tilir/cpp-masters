//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Danger example: see also danger2
//
//----------------------------------------------------------------------------

#include <iostream>

template <typename T> struct D {
  enum { max = 10 };
};

// here we requested instance of D<void>
char buffer[D<void>::max];

extern void clear(char *buf);

int main() {
  std::cout << "Requesting to clean " << D<void>::max << std::endl;
  clear(buffer);
}
