//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Example of notify all
//
//----------------------------------------------------------------------------

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "gtest/gtest.h"

namespace {

std::mutex gmut;
int counter = 0;
std::condition_variable data_cond;

void processing() {
  int c;
  {
    std::unique_lock<std::mutex> lk{gmut};
    data_cond.wait(lk, [] { return counter > 0; });
    // here lock for gmut obtained
    c = counter;
  }
  std::cout << "-";
}

void preparation() {
  {
    std::lock_guard<std::mutex> lk{gmut};
    // here lock for gmut obtained
    counter += 1;
    data_cond.notify_all();
  }
  std::cout << "+";
}

} // namespace

TEST(threads, notify_all) {
  std::thread t2{processing};
  std::thread t3{processing};
  std::thread t4{processing};
  std::thread t1{preparation};
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  std::cout << "\n";
  EXPECT_EQ(counter, 1);
}
