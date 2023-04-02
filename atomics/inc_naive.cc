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
  Counters Val = Cnt.load();
  Val.A += 1;
  Val.B += 1;
  Cnt.store(Val);
}

void threadfunc() {
  for (int i = 0; i < NITER; ++i) {
    increment(ACnt);
  }
}

} // namespace

TEST(atomics, inc_naive) {
  std::vector<std::thread> Threads(NTHREAD);
  for (auto &&t : Threads)
    t = std::thread(threadfunc);
  for (auto &&t : Threads)
    t.join();
  Counters Val = ACnt.load();
  EXPECT_EQ(Val.A, NTHREAD * NITER);
  EXPECT_EQ(Val.B, NTHREAD * NITER);
}