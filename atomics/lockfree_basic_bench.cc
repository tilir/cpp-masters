//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Lock-free thread-safe queue benchmark
//
//----------------------------------------------------------------------------

#include <atomic>
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
    std::cout << "Hello from lock-free queue demo!" << std::endl;
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

std::mutex CoutMutex;

template <typename T> class lf_queue {
  Config Cfg;

  // using unsigned to allow legal wrap around
  struct CellTy {
    std::atomic<unsigned> Sequence;
    T Data;
  };

  std::vector<CellTy> Buffer;
  unsigned BufferMask;
  std::atomic<unsigned> EnqueuePos, DequeuePos;

public:
  lf_queue(Config Cfg)
      : Cfg(Cfg), Buffer(Cfg.BufSize), BufferMask(Cfg.BufSize - 1) {
    if (Cfg.BufSize > (1 << 30))
      throw std::runtime_error("buffer size too large");

    if (Cfg.BufSize < 2)
      throw std::runtime_error("buffer size too small");

    if ((Cfg.BufSize & (Cfg.BufSize - 1)) != 0)
      throw std::runtime_error("buffer size is not power of 2");

    for (unsigned i = 0; i != Cfg.BufSize; ++i)
      Buffer[i].Sequence.store(i);

    EnqueuePos.store(0);
    DequeuePos.store(0);
  }

  bool push(T Data) {
    CellTy *Cell;
    unsigned Pos;
    bool Res = false;

    while (!Res) {
      // fetch the current Position where to enqueue the item
      Pos = EnqueuePos.load();
      Cell = &Buffer[Pos & BufferMask];
      auto Seq = Cell->Sequence.load();
      auto Diff = static_cast<int>(Seq) - static_cast<int>(Pos);

      // queue is full we cannot enqueue and just return false
      // another option: queue moved forward all way round
      if (Diff < 0)
        return false;

      // If its Sequence wasn't touched by other producers
      // check if we can increment the enqueue Position
      if (Diff == 0)
        Res = EnqueuePos.compare_exchange_weak(Pos, Pos + 1);
    }

    // write the item we want to enqueue and bump Sequence
    Cell->Data = std::move(Data);
    Cell->Sequence.store(Pos + 1);
    return true;
  }

  bool pop(T &Data) {
    CellTy *Cell;
    unsigned Pos;
    bool Res = false;

    while (!Res) {
      // fetch the current Position from where we can dequeue an item
      Pos = DequeuePos.load();
      Cell = &Buffer[Pos & BufferMask];
      auto Seq = Cell->Sequence.load();
      auto Diff = static_cast<int>(Seq) - static_cast<int>(Pos + 1);

      // probably the queue is empty, then return false
      if (Diff < 0)
        return false;

      // Check if its Sequence was changed by a producer and wasn't changed by
      // other consumers and if we can increment the dequeue Position
      if (Diff == 0)
        Res = DequeuePos.compare_exchange_weak(Pos, Pos + 1);
    }

    // read the item and update for the next round of the buffer
    Data = std::move(Cell->Data);
    Cell->Sequence.store(Pos + BufferMask + 1);
    return true;
  }

  bool is_empty() const { return EnqueuePos.load() == DequeuePos.load(); }
};

std::atomic<int> NTasks;

void produce(lf_queue<int> &Q, Config Cfg) {
  for (;;) {
    int N = NTasks.load();

    // check if I need enter CAS loop at all
    if (N < 0)
      break;

    while (!NTasks.compare_exchange_weak(N, N - 1)) {
      // check if inside CAS loop other producers exhausted tasks
      if (N < 0)
        return;
      std::this_thread::yield();
    }

    std::this_thread::sleep_for(Cfg.PTime);
    while (!Q.push(N))
      std::this_thread::yield();
  }
}

void consume(lf_queue<int> &Q, Config Cfg) {
  for (;;) {
    int N = NTasks.load();
    if (N < 0 && Q.is_empty())
      break;
    bool Succ = Q.pop(N);
    if (Succ) {
      std::this_thread::sleep_for(Cfg.CTime);
    }
  }
}

} // namespace

int main(int argc, char **argv) try {
  auto Cfg = parse_cfg(argc, argv);

  for (int NMAX = 1; NMAX < Cfg.NThreads; ++NMAX) {
    NTasks = Cfg.NTasks;
    std::vector<std::thread> Producers;
    std::vector<std::thread> Consumers;
    lf_queue<int> Queue(Cfg);

    util::Timer t;
    t.start();

    for (int N = 0; N < NMAX; ++N)
      Producers.emplace_back(produce, std::ref(Queue), Cfg);

    for (int N = 0; N < NMAX; ++N)
      Consumers.emplace_back(consume, std::ref(Queue), Cfg);

    for (auto &P : Producers)
      P.join();

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
