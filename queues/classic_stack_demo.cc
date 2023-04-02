//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Classic thread-safe stack demo
//
// try:
// ./build/stacks/classic_stack_demo
// ./build/stacks/classic_stack_demo -nprod=2
// ./build/stacks/classic_stack_demo -bufsize=1000
// ./build/stacks/classic_stack_demo -bufsize=1000 -nprod=2
// ./build/stacks/classic_stack_demo -nprod=16 -ncons=16 -bufsize=100
// ./build/stacks/classic_stack_demo -nprod=16 -ncons=16 -bufsize=100
//
//----------------------------------------------------------------------------

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stack>
#include <thread>
#include <vector>

#include "opts.hpp"
#include "timer.hpp"

namespace chrono = std::chrono;

namespace {

constexpr bool DEF_VERBOSE = false;
constexpr int DEF_BUFSIZE = 100;
constexpr int DEF_NTASKS = 1000;
constexpr int DEF_NPROD = 1;
constexpr int DEF_NCONS = 1;
constexpr int DEF_PTIME = 1;
constexpr int DEF_CTIME = 1;

struct Config {
  bool Verbose = DEF_VERBOSE;
  int BufSize = DEF_BUFSIZE;
  int NTasks = DEF_NTASKS;
  int NProd = DEF_NPROD;
  int NCons = DEF_NCONS;
  chrono::milliseconds PTime;
  chrono::milliseconds CTime;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.template add<int>("bufsize", DEF_BUFSIZE, "fixed stack size");
  OptParser.template add<int>("ntasks", DEF_NTASKS,
                              "number of tasks to proceed");
  OptParser.template add<int>("nprod", DEF_NPROD, "number of producers");
  OptParser.template add<int>("ncons", DEF_NCONS, "number of consumers");
  OptParser.template add<int>("ptime", DEF_PTIME, "production time");
  OptParser.template add<int>("ctime", DEF_CTIME, "consuming time");
  OptParser.parse(argc, argv);

  Cfg.Verbose = OptParser.exists("verbose");
  Cfg.BufSize = OptParser.template get<int>("bufsize");
  Cfg.NTasks = OptParser.template get<int>("ntasks");
  Cfg.NProd = OptParser.template get<int>("nprod");
  Cfg.NCons = OptParser.template get<int>("ncons");
  int PTime = OptParser.template get<int>("ptime");
  int CTime = OptParser.template get<int>("ctime");

  if (Cfg.BufSize < 1)
    throw std::runtime_error("please specify buffer size >= 1");
  if (Cfg.NTasks < 1)
    throw std::runtime_error("please specify number of tasks >= 1");
  if (Cfg.NProd < 1)
    throw std::runtime_error("please specify number of produsers >= 1");
  if (Cfg.NCons < 1)
    throw std::runtime_error("please specify number of consumers >= 1");
  if (PTime < 0)
    throw std::runtime_error("please specify production time >= 0");
  if (CTime < 0)
    throw std::runtime_error("please specify consuming time >= 0");

  Cfg.PTime = chrono::milliseconds(PTime);
  Cfg.CTime = chrono::milliseconds(CTime);

  if (Cfg.Verbose) {
    std::cout << "Hello from classic stack demo!" << std::endl;
    std::cout << "Buffer size: " << Cfg.BufSize << std::endl;
    std::cout << "Number of tasks: " << Cfg.NTasks << std::endl;
    std::cout << "Number of producers: " << Cfg.NProd << std::endl;
    std::cout << "Number of consumers: " << Cfg.NCons << std::endl;
    std::cout << "Producing time: " << Cfg.PTime.count() << std::endl;
    std::cout << "Consuming time: " << Cfg.CTime.count() << std::endl;
  }

  return Cfg;
}

} // namespace

namespace {

template <typename T> class ts_stack {
  Config Cfg;

  // fixed-size stack
  // look it is unaligned...
  std::vector<T> Buffer;
  int NCur = -1;
  bool Done = false;
  mutable std::mutex Mut;
  std::condition_variable CondCons, CondProd;

  // this interface cannot safely be public
  bool full() const { return NCur >= static_cast<int>(Buffer.size()); }
  bool empty() const { return NCur < 0; }
  bool done() const { return Done; }

public:
  ts_stack(Config Cfg) : Cfg(Cfg), Buffer(Cfg.BufSize) {}

  void push(T Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondProd.wait(Lk, [this] { return !full() || done(); });
    if (Done)
      return;
    NCur += 1;
    Buffer[NCur] = Data;
    if (Cfg.Verbose)
      std::cout << "+";
    CondCons.notify_one();
  }

  void wait_and_pop(T &Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondCons.wait(Lk, [this] { return !empty() || done(); });
    if (empty())
      return;
    Data = Buffer[NCur];
    NCur -= 1;
    if (Cfg.Verbose)
      std::cout << "-";
    CondProd.notify_one(); // wake up producer
  }

  void wake_and_done() {
    Done = true;
    CondCons.notify_all();
    CondProd.notify_all();
  }

  // only for extern use, locks NCur
  // we need this to not stop consume too early
  bool is_empty() const {
    std::unique_lock<std::mutex> Lk{Mut};
    return NCur < 0;
  }
};

int NTasks;
std::mutex TaskMut;

void produce(ts_stack<int> &Q, Config Cfg) {
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
  Q.wake_and_done();
}

void consume(ts_stack<int> &Q, Config Cfg) {
  for (;;) {
    int N;
    // critical section
    {
      std::lock_guard<std::mutex> Lk{TaskMut};
      if (NTasks < 0)
        break;
    }

    // pop, then spend some time consuming
    Q.wait_and_pop(N);
    std::this_thread::sleep_for(Cfg.CTime);
  }
  Q.wake_and_done();
}

} // namespace

int main(int argc, char **argv) try {
  auto Cfg = parse_cfg(argc, argv);
  NTasks = Cfg.NTasks;

  std::vector<std::thread> Producers;
  std::vector<std::thread> Consumers;
  ts_stack<int> Queue(Cfg);

  util::Timer t;
  t.start();

  for (int N = 0; N < Cfg.NProd; ++N)
    Producers.emplace_back(produce, std::ref(Queue), Cfg);

  for (int N = 0; N < Cfg.NCons; ++N)
    Consumers.emplace_back(consume, std::ref(Queue), Cfg);

  for (auto &P : Producers)
    P.join();

  for (auto &C : Consumers)
    C.join();

  t.stop();
  std::cout << "Done: "
            << ": " << t.elapsed_ms() << " millisec" << std::endl;
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << "\n";
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}
