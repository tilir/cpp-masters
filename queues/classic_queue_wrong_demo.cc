//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Classic thread-safe queue demo, hanging version
//
// killing: -ntasks=100000 -ptime=0 -ctime=0 -nprod=4 -ncons=4
// while ./build/queues/classic_queue_wrong_demo -ntasks=100000 -ptime=0 -ctime=0 -nprod=4 -ncons=4; do echo "Ok"; done
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
  OptParser.template add<int>("bufsize", DEF_BUFSIZE, "fixed queue size");
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
    std::cout << "Hello from classic queue demo!" << std::endl;
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

template <typename T> class ts_queue {
  Config Cfg;

  // fixed-size queue
  // look it is unaligned...
  std::vector<T> Buffer;
  int NCur = -1;
  mutable std::mutex Mut;
  std::condition_variable CondCons, CondProd;

  // this interface cannot safely be public
  bool full() const { return NCur >= static_cast<int>(Buffer.size()); }
  bool empty() const { return NCur < 0; }  

public:
  ts_queue(Config Cfg) : Cfg(Cfg), Buffer(Cfg.BufSize) {}

  void push(T Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondProd.wait(Lk, [this] { return !full(); });
    NCur += 1;
    Buffer[NCur] = Data;
    if (Cfg.Verbose)
      std::cout << "+";
    CondCons.notify_one();
  }

  void wait_and_pop(T &Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondCons.wait(Lk, [this] { return !empty(); });
    Data = Buffer[NCur];
    NCur -= 1;
    if (Cfg.Verbose)
      std::cout << "-";
    CondProd.notify_one(); // wake up producer
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
