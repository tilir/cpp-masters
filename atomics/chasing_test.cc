#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

#include "gtest/gtest.h"

volatile long long a = 0, b = 0;
std::atomic<int> wrongcount = 0;
long long MAXCOUNT = 1ll << 10;

// a is always > b
void chase() {
  for (;;) {
    a += 1;
    b += 1;
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

TEST(atomics, chase) {
  std::thread t1{chase};
  std::thread t2{check};
  t1.join();
  t2.join();
  EXPECT_EQ(wrongcount, 0);
}
