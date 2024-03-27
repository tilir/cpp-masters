//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Race demo
//
//----------------------------------------------------------------------------

#include <iostream>
#include <thread>

namespace {

int x = 0;

void __attribute__((noinline)) use(int c) { asm(""::"r"(c)); }

void race() {
  // going up
  for (int i = 0; i < 1000000; ++i) {
    x += 1;
    use(x);
  }

  // going down
  for (int i = 0; i < 1000000; ++i) {
    x -= 1;
    use(x);
  }
}

} // namespace

int main() {
  std::thread t1{race};
  std::thread t2{race};
  t1.join();
  t2.join();
  std::cout << x << std::endl;
}
