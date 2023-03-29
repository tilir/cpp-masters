//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// std::latch solution for prepare and run problem
//
//----------------------------------------------------------------------------

#include <future>
#include <iostream>
#include <latch>
#include <thread>
#include <utility>

#include "gtest/gtest.h"

namespace chrono = std::chrono;

namespace {

constexpr int NSLEEP = 1;

int G = 1;

void prepare(std::latch &L, std::latch &BackL) {
  std::this_thread::sleep_for(chrono::milliseconds(NSLEEP));
  EXPECT_EQ(G, 1);
  L.count_down();
  BackL.wait(); // --- hold here ---
  EXPECT_EQ(G, 2);
}

} // namespace

TEST(queue, latch) {
  std::latch L(2);
  std::latch BackL(1);

  std::thread Fst(prepare, std::ref(L), std::ref(BackL));
  std::thread Snd(prepare, std::ref(L), std::ref(BackL));

  // wait everybody ready
  L.wait();

  G = 2;

  BackL.count_down(); // resume work with latch

  Fst.join();
  Snd.join();
}