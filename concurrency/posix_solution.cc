//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// attempt to solve POSIX style
//
//----------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <unistd.h>

#include "gtest/gtest.h"

namespace {

volatile int x = 0;

void __attribute__((noinline)) use(int c) { asm(""); }

int stick_this_thread_to_core(int core_id) {
  int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
  if (core_id < 0 || core_id >= num_cores)
    return EINVAL;

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  pthread_t current_thread = pthread_self();
  return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

void race() {
  stick_this_thread_to_core(0);

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

TEST(threads, posix_solution) {
  std::thread t1{race};
  std::thread t2{race};
  t1.join();
  t2.join();
  EXPECT_EQ(x, 0);
}
