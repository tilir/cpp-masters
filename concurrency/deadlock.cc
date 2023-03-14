//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Deadlock demo
//
//----------------------------------------------------------------------------

#include <iostream>
#include <mutex>
#include <thread>

#include "gtest/gtest.h"

namespace {

template <typename T> struct holder {
  std::mutex lock_;
  T data_;
  holder(T data) : lock_{}, data_(data) {}
};

template <typename T> void swapholders(holder<T> &lhs, holder<T> &rhs) {
  if (&lhs == &rhs)
    return;

  std::lock_guard<std::mutex> lk1{lhs.lock_};
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::lock_guard<std::mutex> lk2{rhs.lock_};

  std::swap(lhs.data_, rhs.data_);
}

} // namespace

TEST(threads, deadlock) {
  holder<int> x(1), y(2);

  std::thread t1(swapholders<int>, std::ref(x), std::ref(y));
  std::thread t2(swapholders<int>, std::ref(y), std::ref(x));

  t1.join();
  t2.join();
  EXPECT_EQ(0, 0);
}
