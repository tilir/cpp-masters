#pragma once

#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

namespace chrono = std::chrono;

// milliseconds, microsecond and nanoseconds
static const double msec_per_sec = 1000.0;
static const double usec_per_sec = msec_per_sec * msec_per_sec;
static const double nsec_per_sec = msec_per_sec * msec_per_sec * msec_per_sec;

namespace util {

class Timer {
  chrono::high_resolution_clock::time_point Start, Fin;
  bool Started = false;

public:
  Timer() = default;
  void start() {
    assert(!Started);
    Started = true;
    Start = chrono::high_resolution_clock::now();
  }
  void stop() {
    assert(Started);
    Started = false;
    Fin = chrono::high_resolution_clock::now();
  }
  unsigned elapsed_ms() {
    assert(!Started);
    auto Elps = Fin - Start;
    auto Msec = chrono::duration_cast<chrono::milliseconds>(Elps);
    return Msec.count();
  }
};

}

template <typename T, typename ... Args>
inline void measure(std::string_view prefix, T callable, Args&& ... args) {
  util::Timer t;
  t.start();
  std::invoke(callable, std::forward<Args>(args)...);
  t.stop();
  std::cout << prefix << ": " << t.elapsed_ms() << std::endl;
}

template <typename T, typename... Args>
inline void measure_multithread(int TMin, int TMax, T callable,
                                Args &&... args) {
  util::Timer t;
  for (int nthr = TMin; nthr <= TMax; ++nthr) {
    t.start();

    std::vector<std::thread> threads(nthr);
    for (int i = 0; i < nthr; ++i)
      threads[i] = std::thread(callable, std::forward<Args>(args)...);

    for (int i = 0; i < nthr; ++i)
      threads[i].join();

    t.stop();
    std::cout << nthr << " " << t.elapsed_ms() << std::endl;
  }
}

#define noopt(i) asm(""::"r"(i))