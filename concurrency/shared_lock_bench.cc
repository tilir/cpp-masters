//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Benchmark: unique vs shared lock
// quick-bench not support MT:
// https://quick-bench.com/q/sdeGcN3y2TBDoJNOlzBZGqBn_JQ
//
//----------------------------------------------------------------------------

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

#include "timer.hpp"

struct Unique {
  mutable std::mutex m;
  int value;

  int get() const {
    std::unique_lock<std::mutex> lock{m};
    return value;
  }

  void modify(int newval) {
    std::unique_lock<std::mutex> lock{m};
    value = newval;
  }
};

struct Shared {
  mutable std::shared_mutex m;
  int value;

  int get() const {
    std::shared_lock<std::shared_mutex> lock{m};
    return value;
  }

  void modify(int newval) {
    std::unique_lock<std::shared_mutex> lock{m};
    value = newval;
  }
};

constexpr int nget = 1000000;
constexpr int nmod = 1000000;

template <typename T> void modifier(T &t) {
  for (int i = 0; i < nmod; i += 1) {
    if ((i % 1000) != 0)
      continue;
    t.modify(i);
    noopt(i);
  }
}

template <typename T> void getter(const T &t) {
  for (int i = 0; i < nget; i += 1) {
    int k = t.get();
    noopt(k);
    noopt(i);
  }
}

static void UniqueLock() {
  Unique u;
  std::thread t0(modifier<Unique>, std::ref(u));
  std::thread t1(getter<Unique>, std::cref(u));
  std::thread t2(getter<Unique>, std::cref(u));
  std::thread t3(getter<Unique>, std::cref(u));
  std::thread t4(getter<Unique>, std::cref(u));
  std::thread t5(getter<Unique>, std::cref(u));
  t0.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
}

static void SharedLock() {
  Shared u;
  std::thread t0(modifier<Shared>, std::ref(u));
  std::thread t1(getter<Shared>, std::cref(u));
  std::thread t2(getter<Shared>, std::cref(u));
  std::thread t3(getter<Shared>, std::cref(u));
  std::thread t4(getter<Shared>, std::cref(u));
  std::thread t5(getter<Shared>, std::cref(u));
  t0.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
}

int main() {
#ifndef CHECK_STRACE
  measure("UniqueLock", UniqueLock);
  measure("SharedLock", SharedLock);
#else
  printf("Start\n");
  Unique u;
  Shared s;
  int *p;
  int v;
  p = new int; // SBRK
  noopt(p);
  v = u.get();
  noopt(v);
  p = new int;
  noopt(p);
  u.modify(1);
  p = new int;
  noopt(p);
  v = s.get();
  noopt(v);
  p = new int;
  noopt(p);
  s.modify(1);
  noopt(s.value);
#endif
}
