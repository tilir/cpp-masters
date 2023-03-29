//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Unbounded thread-safe queue demo
//
// try:
// ./build/queues/unbounded_queue_demo
//
//----------------------------------------------------------------------------

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "opts.hpp"
#include "timer.hpp"

namespace chrono = std::chrono;

namespace {

constexpr bool DEF_VERBOSE = false;
constexpr int DEF_NTASKS = 1000;
constexpr int DEF_NPROD = 1;
constexpr int DEF_NCONS = 1;
constexpr int DEF_PTIME = 1;
constexpr int DEF_CTIME = 1;

struct Config {
  bool Verbose = DEF_VERBOSE;
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
  OptParser.template add<int>("ntasks", DEF_NTASKS,
                              "number of tasks to proceed");
  OptParser.template add<int>("nprod", DEF_NPROD, "number of producers");
  OptParser.template add<int>("ncons", DEF_NCONS, "number of consumers");
  OptParser.template add<int>("ptime", DEF_PTIME, "production time");
  OptParser.template add<int>("ctime", DEF_CTIME, "consuming time");
  OptParser.parse(argc, argv);

  Cfg.Verbose = OptParser.exists("verbose");
  Cfg.NTasks = OptParser.template get<int>("ntasks");
  Cfg.NProd = OptParser.template get<int>("nprod");
  Cfg.NCons = OptParser.template get<int>("ncons");
  int PTime = OptParser.template get<int>("ptime");
  int CTime = OptParser.template get<int>("ctime");

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
    std::cout << "Hello from classic queue demo!" << std::endl;
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

template <typename T> T Limiter();

template <> int Limiter<int>() { return -1; }

template <typename T> class ts_queue {
  Config Cfg;

  // unbounded queue
  std::queue<T> Q;
  mutable std::mutex Mut;
  std::condition_variable Cond;

public:
  ts_queue(Config Cfg) : Cfg(Cfg) {}

  void push(T Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    Q.push(std::move(Data));
    Cond.notify_one();
  }

  void wait_and_pop(T &Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    Cond.wait(Lk, [this] { return !Q.empty(); });
    T Task = std::move(Q.front());
    if (Task == Limiter<T>())
      return;
    Data = Task;
    Q.pop();
  }

  void wake_and_done() {
    Q.push(Limiter<T>());
    Cond.notify_all();
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
  Q.wake_and_done();
}

void consume(ts_queue<int> &Q, Config Cfg) {
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
  ts_queue<int> Queue(Cfg);

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
