//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// std::mutex vs two types of custom futexes
//
// ./build/atomics/custom_mutex_demo -type=0
// ./build/atomics/custom_mutex_demo -type=1
// ./build/atomics/custom_mutex_demo -type=2
//
//----------------------------------------------------------------------------

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "opts.hpp"
#include "timer.hpp"

namespace chrono = std::chrono;

namespace {

constexpr bool DEF_VERBOSE = false;
constexpr int DEF_WORKLOAD = 20000000;
constexpr int DEF_NTHRMIN = 1;
constexpr int DEF_NTHRMAX = 10;
constexpr int DEF_TYPE = 0;

struct Config {
  bool Verbose = DEF_VERBOSE;
  int Workload = DEF_WORKLOAD;
  int NThrmin = DEF_NTHRMIN;
  int NThrmax = DEF_NTHRMAX;
  int Type = DEF_TYPE;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.template add<int>("workload", DEF_WORKLOAD,
                              "number of elts to count");
  OptParser.template add<int>("min", DEF_NTHRMIN, "start number of threads");
  OptParser.template add<int>("max", DEF_NTHRMAX, "end number of threads");
  OptParser.template add<int>("type", DEF_TYPE,
                              "0 = std::mutex, 1 = futex, 2 = futex_yield");
  OptParser.parse(argc, argv);

  Cfg.Verbose = OptParser.exists("verbose");
  Cfg.Workload = OptParser.template get<int>("workload");
  Cfg.NThrmin = OptParser.template get<int>("min");
  Cfg.NThrmax = OptParser.template get<int>("max");
  Cfg.Type = OptParser.template get<int>("type");

  if (Cfg.Workload < 1)
    throw std::runtime_error("please specify workload size >= 1");
  if (Cfg.NThrmin < 1)
    throw std::runtime_error("please specify min number of threads >= 1");
  if (Cfg.NThrmax < 1)
    throw std::runtime_error("please specify max number of threads >= 1");
  if (Cfg.NThrmax < Cfg.NThrmin)
    throw std::runtime_error(
        "please specify max number of threads >= min number of threads");
  if (Cfg.Type < 0 || Cfg.Type > 2)
    throw std::runtime_error("please specify type in {0, 1, 2}");

  if (Cfg.Verbose) {
    std::cout << "Hello from counter demo!" << std::endl;
    std::cout << "Workload size: " << Cfg.Workload << std::endl;
    std::cout << "Min threads: " << Cfg.NThrmin << std::endl;
    std::cout << "Max threads: " << Cfg.NThrmax << std::endl;
    std::cout << "Type: " << Cfg.Type << std::endl;
  }

  return Cfg;
}

} // namespace

namespace {

// greedy futex
class futex {
  std::atomic_flag flag{ATOMIC_FLAG_INIT};

public:
  void lock() {
    while (flag.test_and_set())
      ;
  }

  bool try_lock() { return !flag.test_and_set(); }
  void unlock() { flag.clear(); }
};

// gentle futex
class futex_yield {
  std::atomic_flag flag{ATOMIC_FLAG_INIT};

public:
  void lock() {
    while (flag.test_and_set())
      std::this_thread::yield();
  }

  bool try_lock() { return !flag.test_and_set(); }
  void unlock() { flag.clear(); }
};

int MCnt = 0;
std::mutex M;
futex F;
futex_yield FY;

void threadfunc_m(int Wload) {
  MCnt = 0;
  for (;;) {
    std::lock_guard<std::mutex> L{M};
    MCnt += 1;
    if (MCnt >= Wload)
      break;
  }
}

void threadfunc_f(int Wload) {
  MCnt = 0;
  for (;;) {
    std::lock_guard<futex> L{F};
    MCnt += 1;
    if (MCnt >= Wload)
      break;
  }
}

void threadfunc_fy(int Wload) {
  MCnt = 0;
  for (;;) {
    std::lock_guard<futex_yield> L{FY};
    MCnt += 1;
    if (MCnt >= Wload)
      break;
  }
}

} // namespace

int main(int argc, char **argv) {
  auto Cfg = parse_cfg(argc, argv);
  switch (Cfg.Type) {
  case 0:
    measure_multithread(Cfg.NThrmin, Cfg.NThrmax, threadfunc_m, Cfg.Workload);
    break;
  case 1:
    measure_multithread(Cfg.NThrmin, Cfg.NThrmax, threadfunc_f, Cfg.Workload);
    break;
  case 2:
    measure_multithread(Cfg.NThrmin, Cfg.NThrmax, threadfunc_fy, Cfg.Workload);
    break;
  }
}
