//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Parallel reduce benchmark
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <execution>
#include <future>
#include <iterator>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

#include "opts.hpp"
#include "timer.hpp"

namespace chrono = std::chrono;
namespace exec = std::execution;

namespace {

constexpr bool DEF_VERBOSE = false;
constexpr int DEF_NSTART = 10000000;
constexpr int DEF_NSTEP = 10000000;
constexpr int DEF_STEPS = 10;
constexpr const char *DEF_POLICY = "seq";
constexpr const char *DEF_ALG = "reduce";

enum class Policy { Seq, Par, Par_Unseq, Wrong };

constexpr auto getPolicy(std::string P) {
  std::transform(P.begin(), P.end(), P.begin(),
                 [](unsigned char C) { return std::tolower(C); });
  if (P == "seq")
    return Policy::Seq;
  if (P == "par")
    return Policy::Par;
  if (P == "par_unseq")
    return Policy::Par_Unseq;
  return Policy::Wrong;
}

enum class Alg { Reduce, Naive, Async, Wrong };

constexpr auto getAlg(std::string P) {
  std::transform(P.begin(), P.end(), P.begin(),
                 [](unsigned char C) { return std::tolower(C); });
  if (P == "reduce")
    return Alg::Reduce;
  if (P == "naive")
    return Alg::Naive;
  if (P == "async")
    return Alg::Async;
  return Alg::Wrong;
}

struct Config {
  bool Verbose = DEF_VERBOSE;
  int NStart = DEF_NSTART;
  int NStep = DEF_NSTEP;
  int Steps = DEF_STEPS;
  Policy P = getPolicy(DEF_POLICY);
  Alg A = getAlg(DEF_ALG);
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.template add<int>("nstart", DEF_NSTART, "elts to start with");
  OptParser.template add<int>("nstep", DEF_NSTEP, "elts for each step");
  OptParser.template add<int>("steps", DEF_STEPS, "number of steps");
  OptParser.template add<std::string>("policy", DEF_POLICY,
                                      "policy (seq, par, par_unseq)");
  OptParser.template add<std::string>("alg", DEF_ALG,
                                      "algorithm (reduce, naive, async)");
  OptParser.parse(argc, argv);

  Cfg.Verbose = OptParser.exists("verbose");
  Cfg.NStart = OptParser.template get<int>("nstart");
  Cfg.NStep = OptParser.template get<int>("nstep");
  Cfg.Steps = OptParser.template get<int>("steps");
  std::string P = OptParser.template get<std::string>("policy");
  std::string A = OptParser.template get<std::string>("alg");

  Cfg.P = getPolicy(P);
  Cfg.A = getAlg(A);

  if (Cfg.NStart < 1)
    throw std::runtime_error("please specify start size >= 1");
  if (Cfg.NStep < 1)
    throw std::runtime_error("please specify step size >= 1");
  if (Cfg.Steps < 1)
    throw std::runtime_error("please specify num of steps >= 1");
  if (Cfg.P == Policy::Wrong)
    throw std::runtime_error(
        "please specify policy to be seq, par or par_unseq");
  if (Cfg.A == Alg::Wrong)
    throw std::runtime_error("sorry, algorithm unsupported");

  if (Cfg.Verbose) {
    std::cout << "Hello from reduce_bench demo!" << std::endl;
    std::cout << "Start size: " << Cfg.NStart << std::endl;
    std::cout << "Step size: " << Cfg.NStep << std::endl;
    std::cout << "Number of steps: " << Cfg.Steps << std::endl;
    std::cout << "Alg: " << A << std::endl;
    std::cout << "Policy: " << P << std::endl;
  }

  return Cfg;
}

} // namespace

namespace {

constexpr int MINLEN = 25;

unsigned determine_threads(unsigned length) {
  const unsigned long min_per_thread = MINLEN;
  unsigned long max_threads = length / min_per_thread;
  unsigned long hardware_conc = std::thread::hardware_concurrency();
  return std::min(hardware_conc != 0 ? hardware_conc : 2, max_threads);
}

template <typename Iterator,
          typename T = typename std::iterator_traits<Iterator>::value_type>
T naive_accumulate(Iterator first, Iterator last, T init = 0) {
  long length = distance(first, last);
  if (0 == length)
    return init;
  if (length <= MINLEN)
    return std::accumulate(first, last, init);

  static unsigned nthreads = determine_threads(length);
  long bsize = length / nthreads;

  std::vector<std::thread> threads(nthreads);
  std::vector<T> results(nthreads + 1);

  auto accumulate_block = [](Iterator first, Iterator last, T &result) {
    result = std::accumulate(first, last, result);
  };

  unsigned tidx = 0;

  for (; length >= bsize * (tidx + 1); first += bsize, tidx += 1)
    threads[tidx] = std::thread(accumulate_block, first, first + bsize,
                                std::ref(results[tidx]));

  auto remainder = length - bsize * tidx;

  if (remainder > 0) {
    assert(tidx == nthreads);
    accumulate_block(first, first + remainder, std::ref(results[tidx]));
  }

  for (auto &&t : threads)
    t.join();

  return std::accumulate(results.begin(), results.end(), init);
}

template <typename Iterator,
          typename T = typename std::iterator_traits<Iterator>::value_type>
T async_accumulate(Iterator first, Iterator last, T init = 0) {
  long length = distance(first, last);
  if (0 == length)
    return init;
  if (length <= MINLEN)
    return std::accumulate(first, last, init);

  static unsigned nthreads = determine_threads(length);
  long bsize = length / nthreads;

  std::vector<std::future<T>> results(nthreads);

  auto accumulate_block = [](Iterator first, Iterator last) {
    return std::accumulate(first, last, T{});
  };

  unsigned tidx = 0;

  for (; length >= bsize * (tidx + 1); first += bsize, tidx += 1)
    results[tidx] = std::async(accumulate_block, first, first + bsize);

  auto remainder = length - bsize * tidx;

  T result = init;

  if (remainder > 0) {
    assert(tidx < nthreads);
    result += accumulate_block(first, first + remainder);
  }

  for (unsigned long i = 0; i < nthreads; ++i)
    result += results[i].get();

  return result;
}

constexpr int REDUCE_MULT = 100;

int measureReduce(auto It, long Sz, auto P) {
  for (int I = 0; I < REDUCE_MULT; ++I) {
    auto N = std::reduce(P, It, It + Sz);
    noopt(N);
    noopt(I);
  }
  return REDUCE_MULT;
}

int measureNaive(auto It, long Sz, auto P) {
  for (int I = 0; I < REDUCE_MULT; ++I) {
    auto N = naive_accumulate(It, It + Sz);
    noopt(N);
    noopt(I);
  }
  return REDUCE_MULT;
}

int measureAsync(auto It, long Sz, auto P) {
  for (int I = 0; I < REDUCE_MULT; ++I) {
    auto N = async_accumulate(It, It + Sz);
    noopt(N);
    noopt(I);
  }
  return REDUCE_MULT;
}

// returns divisor for linear algs
int measureAlg(auto It, long Sz, Alg A, auto P) {
  switch (A) {
  case Alg::Reduce:
    return measureReduce(It, Sz, P);
  case Alg::Naive:
    return measureNaive(It, Sz, P);
  case Alg::Async:
    return measureAsync(It, Sz, P);
  default:
    throw std::runtime_error("Alg not supported");
  }
}

} // namespace

int main(int argc, char **argv) try {
  auto Cfg = parse_cfg(argc, argv);
  std::random_device RD;
  std::mt19937 MT(RD());

  for (long N = 0; N < Cfg.Steps; ++N) {
    long Sz = Cfg.NStart + Cfg.NStep * N;
    std::vector<int> V(Sz);
    std::iota(V.begin(), V.end(), 0);
    std::shuffle(V.begin(), V.end(), MT);
    util::Timer t;
    double D;
    t.start();
    switch (Cfg.P) {
    case Policy::Seq:
      D = measureAlg(V.begin(), Sz, Cfg.A, exec::seq);
      break;
    case Policy::Par:
      D = measureAlg(V.begin(), Sz, Cfg.A, exec::par);
      break;
    case Policy::Par_Unseq:
      D = measureAlg(V.begin(), Sz, Cfg.A, exec::par_unseq);
      break;
    default:
      throw std::runtime_error("Policy not suppported");
    };
    t.stop();

    // elapsed ms may be too large, divisor applied
    std::cout << Sz << " " << t.elapsed_ms() / D << std::endl;
  }
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << "\n";
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}
