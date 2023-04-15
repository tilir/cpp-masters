// g++-11 -O2 --std=c++20 atomics/chasing_demo.cc -pthread

#include <atomic>
#include <iostream>
#include <thread>

volatile long long a = 0, b = 0;
std::atomic<int> wrongcount = 0;
long long MAXCOUNT = 1ll << 20;

// a is always > b
void chase() {
  for (;;) {
    a = a + 1;
    b = b + 1;
    if (a > MAXCOUNT)
      break;
  }
}

// check that a > b
void check() {
  for (;;) {
    long long bval = b;
    long long aval = a;
    if (aval < bval) {
      wrongcount += 1;
      break;
    }
    if (aval > MAXCOUNT)
      break;
  }
}

int main() {
  std::thread t1{chase};
  std::thread t2{check};
  t1.join();
  t2.join();
  auto w = wrongcount.load();
  if (w != 0)
    std::cout << "Wrong " << w << "times\n";
  return w;
}
