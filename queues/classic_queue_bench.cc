//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Classic thread-safe queue benchmark
//
//----------------------------------------------------------------------------

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "opts.hpp"
#include "timer.hpp"

namespace chrono = std::chrono;

namespace {

constexpr bool DEF_VERBOSE = false;
constexpr int DEF_BUFSIZE = 1024;
constexpr int DEF_NTASKS = 1000000;
constexpr int DEF_MAX_THREADS = 10;
constexpr int DEF_PTIME = 0;
constexpr int DEF_CTIME = 0;

struct Config {
  bool Verbose = DEF_VERBOSE;
  int BufSize = DEF_BUFSIZE;
  int NTasks = DEF_NTASKS;
  int NThreads = DEF_MAX_THREADS;
  chrono::milliseconds PTime;
  chrono::milliseconds CTime;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.template add<int>("bufsize", DEF_BUFSIZE, "fixed queue size");
  OptParser.template add<int>("ntasks", DEF_NTASKS,
                              "number of tasks to proceed");
  OptParser.template add<int>("nthreads", DEF_MAX_THREADS,
                              "max number of prod + cons");
  OptParser.template add<int>("ptime", DEF_PTIME, "production time");
  OptParser.template add<int>("ctime", DEF_CTIME, "consuming time");
  OptParser.parse(argc, argv);

  Cfg.Verbose = OptParser.exists("verbose");
  Cfg.BufSize = OptParser.template get<int>("bufsize");
  Cfg.NTasks = OptParser.template get<int>("ntasks");
  Cfg.NThreads = OptParser.template get<int>("nthreads");
  int PTime = OptParser.template get<int>("ptime");
  int CTime = OptParser.template get<int>("ctime");

  if (Cfg.BufSize < 1)
    throw std::runtime_error("please specify buffer size >= 1");
  if (Cfg.NTasks < 1)
    throw std::runtime_error("please specify number of tasks >= 1");
  if (Cfg.NThreads < 1)
    throw std::runtime_error("please specify max number of threads >= 1");
  if (PTime < 0)
    throw std::runtime_error("please specify production time >= 0");
  if (CTime < 0)
    throw std::runtime_error("please specify consuming time >= 0");

  Cfg.PTime = chrono::milliseconds(PTime);
  Cfg.CTime = chrono::milliseconds(CTime);

  if (Cfg.Verbose) {
    std::cout << "Hello from queue demo!" << std::endl;
    std::cout << "Buffer size: " << Cfg.BufSize << std::endl;
    std::cout << "Number of tasks: " << Cfg.NTasks << std::endl;
    std::cout << "Number of threads: " << Cfg.NThreads << std::endl;
    std::cout << "Producing time: " << Cfg.PTime.count() << std::endl;
    std::cout << "Consuming time: " << Cfg.CTime.count() << std::endl;
  }

  return Cfg;
}

} // namespace

namespace {

template <typename T> class ts_queue {
  Config Cfg;

  // fixed-size queue
  // look it is unaligned...
  alignas(64) std::vector<T> Buffer;
  alignas(64) int NCur = -1;
  alignas(64) int NRel = 0; // start position
  alignas(64) bool Done = false;
  alignas(64) mutable std::mutex Mut;
  alignas(64) std::condition_variable CondCons, CondProd;

  // this interface cannot safely be public
  bool full() const { return NCur >= static_cast<int>(Buffer.size()); }
  bool empty() const { return NCur < 0; }
  bool done() const { return Done; }

public:
  ts_queue(Config Cfg) : Cfg(Cfg), Buffer(Cfg.BufSize) {
    // prevent integer overflow cases
    if (Cfg.BufSize > (1 << 30))
      throw std::runtime_error("unsupported buffer size");
  }

  void push(T Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondProd.wait(Lk, [this] { return !full(); });

    // exception safety
    int NewCur = NCur + 1;
    Buffer[(NRel + NewCur) % Buffer.size()] = Data;
    NCur = NewCur;
    Lk.unlock();
    CondCons.notify_one();
  }

  bool wait_and_pop(T &Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondCons.wait(Lk, [this] { return !empty() || done(); });
    if (empty())
      return false;
    Data = Buffer[NRel % Buffer.size()];
    NRel = (NRel + 1) % Buffer.size();
    NCur -= 1;
    Lk.unlock();
    CondProd.notify_one();
    return true;
  }

  void wake_and_done() {
    std::unique_lock<std::mutex> Lk{Mut};
    Done = true;
    Lk.unlock();
    CondCons.notify_all();
  }

  // only for extern use, locks NCur
  // we need this to not stop consume too early
  bool is_empty_and_done() const {
    std::unique_lock<std::mutex> Lk{Mut};
    return (NCur < 0) && Done;
  }
};

int NTasks;
std::mutex TaskMut;

void produce(ts_queue<int> &Q, Config Cfg) {
  for (;;) {
    int N;
    // critical section
    {
      std::lock_guard<std::mutex> Lk{TaskMut};
      if (NTasks < 0)
        break;
      N = NTasks;
      NTasks -= 1;
    }

    // spend some time producing, then push
    std::this_thread::sleep_for(Cfg.PTime);
    Q.push(N);
  }
}

void consume(ts_queue<int> &Q, Config Cfg) {
  for (;;) {
    // critical section
    {
      std::lock_guard<std::mutex> Lk{TaskMut};
      if (NTasks < 0 && Q.is_empty_and_done())
        break;
    }

    // pop, then spend some time consuming
    // if pop succesfull
    int N;
    bool Succ = Q.wait_and_pop(N);
    if (Succ)
      std::this_thread::sleep_for(Cfg.CTime);
  }
}

} // namespace

int main(int argc, char **argv) try {
  auto Cfg = parse_cfg(argc, argv);

  for (int NMAX = 1; NMAX < Cfg.NThreads; ++NMAX) {
    NTasks = Cfg.NTasks;
    std::vector<std::thread> Producers;
    std::vector<std::thread> Consumers;
    ts_queue<int> Queue(Cfg);

    util::Timer t;
    t.start();

    for (int N = 0; N < NMAX; ++N)
      Producers.emplace_back(produce, std::ref(Queue), Cfg);

    for (int N = 0; N < NMAX; ++N)
      Consumers.emplace_back(consume, std::ref(Queue), Cfg);

    for (auto &P : Producers)
      P.join();

    // after all producers join
    Queue.wake_and_done();

    for (auto &C : Consumers)
      C.join();

    t.stop();
    std::cout << NMAX << " " << t.elapsed_ms() << std::endl;
  }
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << "\n";
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}
