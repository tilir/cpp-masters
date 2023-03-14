//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Naive solution to dining philosophers
//
//----------------------------------------------------------------------------

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

namespace {

std::mutex mstdout;

void mylock(std::mutex &left, std::mutex &right) {
  for (;;) {
    left.lock();
    if (right.try_lock())
      break;
    left.unlock();
    std::this_thread::yield();
  }
}

constexpr int numdiners = 6;
constexpr int numeats = 10;
constexpr int eattime = 100;
constexpr int thinktime = 100;

class Philosopher {
  int num_;
  int full_ = 0;
  std::mutex &left_, &right_;

public:
  Philosopher(int num, std::mutex &left, std::mutex &right)
      : num_(num), left_(left), right_(right) {}

  void msgout() const {
    std::lock_guard<std::mutex> lk{mstdout};
    std::cout << num_ << " eating" << std::endl;
  }

  void eat() {
    // take forks from left and right
    mylock(left_, right_);

    // at this point forks are taken
    // msgout();
    full_ += 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(eattime));
    left_.unlock();
    right_.unlock();
  }

  int num() const { return num_; }
  int full() const { return full_; }

  void dine() {
    for (int i = 0; i < numeats; ++i) {
      eat();
      std::this_thread::sleep_for(std::chrono::milliseconds(thinktime));
    }
  }
};

} // namespace

TEST(threads, dining_naive) {
  std::vector<std::mutex> forks(numdiners);
  std::vector<Philosopher> diners;
  for (int i = 0; i < numdiners; ++i) {
    int j = (i == (numdiners - 1)) ? 0 : i + 1;
    diners.emplace_back(i + 1, forks[i], forks[j]);
  }

  std::vector<std::thread> threads(numdiners);
  for (int i = 0; i < numdiners; ++i) {
    threads[i] = std::thread(&Philosopher::dine, std::ref(diners[i]));
  }
  for (auto &t : threads)
    t.join();

  for (auto d : diners)
    EXPECT_EQ(d.full(), numeats);
}
