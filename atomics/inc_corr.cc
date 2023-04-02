#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

namespace {

constexpr int NTHREAD = 8;
constexpr int NITER = 1000;

struct Counters {
  int A = 0, B = 0;
};

std::atomic<Counters> ACnt;

void increment(std::atomic<Counters> &Cnt) {
  Counters Cnl = ACnt.load(), Cnn;
  do {
    if (Cnl.A >= NTHREAD * NITER)
      break;
    Cnn = {Cnl.A + 1, Cnl.B + 1};
  } while (!ACnt.compare_exchange_weak(Cnl, Cnn));
}

void threadfunc() {
  for (int i = 0; i < NITER; ++i) {
    increment(ACnt);
  }
}

} // namespace

TEST(atomics, inc_corr) {
  std::vector<std::thread> Threads(NTHREAD);
  for (auto &&t : Threads)
    t = std::thread(threadfunc);
  for (auto &&t : Threads)
    t.join();
  Counters Val = ACnt.load();
  EXPECT_EQ(Val.A, NTHREAD * NITER);
  EXPECT_EQ(Val.B, NTHREAD * NITER);
}