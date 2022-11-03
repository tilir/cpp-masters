//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// CE Collatz numbers
//
//-----------------------------------------------------------------------------

#include <iostream>

constexpr long long maxcollatz(int n) {
  long long cmax = 0, xmax = 0;
  for (int x = 2; x < n; ++x) {
    long long y = x;
    long long c = 0;
    while (y > 1) {
      c += 1;
      if ((y % 2) == 0)
        y = y / 2;
      else
        y = 3 * y + 1;
    }
    if (c > cmax) {
      cmax = c;
      xmax = x;
    }
  }
  return xmax;
}

#ifndef MAXC
#define MAXC 100
#endif

int main() {
  constexpr long long CT = maxcollatz(MAXC);
  std::cout << "CT: " << CT << std::endl;
  std::cout << "RT: " << maxcollatz(MAXC) << std::endl;
}
