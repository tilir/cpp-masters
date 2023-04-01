//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Atomic vs mutex counter
//
// ./build/atomics/atomic_counter_demo
// ./build/atomics/atomic_counter_demo -atomic=1
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
constexpr int DEF_ATOMIC = 0;

struct Config {
  bool Verbose = DEF_VERBOSE;
  int Workload = DEF_WORKLOAD;
  int NThrmin = DEF_NTHRMIN;
  int NThrmax = DEF_NTHRMAX;
  int Atomic = DEF_ATOMIC;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.template add<int>("workload", DEF_WORKLOAD,
                              "number of elts to count");
  OptParser.template add<int>("min", DEF_NTHRMIN, "start number of threads");
  OptParser.template add<int>("max", DEF_NTHRMAX, "end number of threads");
  OptParser.template add<int>("atomic", DEF_ATOMIC, "use atomic count if 1");
  OptParser.parse(argc, argv);

  Cfg.Verbose = OptParser.exists("verbose");
  Cfg.Workload = OptParser.template get<int>("workload");
  Cfg.NThrmin = OptParser.template get<int>("min");
  Cfg.NThrmax = OptParser.template get<int>("max");
  Cfg.Atomic = OptParser.template get<int>("atomic");

  if (Cfg.Workload < 1)
    throw std::runtime_error("please specify workload size >= 1");
  if (Cfg.NThrmin < 1)
    throw std::runtime_error("please specify min number of threads >= 1");
  if (Cfg.NThrmax < 1)
    throw std::runtime_error("please specify max number of threads >= 1");
  if (Cfg.NThrmax < Cfg.NThrmin)
    throw std::runtime_error(
        "please specify max number of threads >= min number of threads");

  if (Cfg.Verbose) {
    std::cout << "Hello from counter demo!" << std::endl;
    std::cout << "Workload size: " << Cfg.Workload << std::endl;
    std::cout << "Min threads: " << Cfg.NThrmin << std::endl;
    std::cout << "Max threads: " << Cfg.NThrmax << std::endl;
    std::cout << "Usage of atomics: " << Cfg.Atomic << std::endl;
  }

  return Cfg;
}

} // namespace

namespace {

std::atomic<int> ACnt{0};

void threadfunc_atomic(int Wload) {
  ACnt = 0;
  for (;;) {
    ACnt += 1;
    if (ACnt >= Wload)
      break;
  }
}

int MCnt = 0;
std::mutex M;

void threadfunc(int Wload) {
  MCnt = 0;
  for (;;) {
    std::lock_guard<std::mutex> L{M};
    MCnt += 1;
    if (MCnt >= Wload)
      break;
  }
}

} // namespace

int main(int argc, char **argv) {
  auto Cfg = parse_cfg(argc, argv);
  if (Cfg.Atomic > 0)
    measure_multithread(Cfg.NThrmin, Cfg.NThrmax, threadfunc_atomic,
                        Cfg.Workload);
  else
    measure_multithread(Cfg.NThrmin, Cfg.NThrmax, threadfunc, Cfg.Workload);
}
