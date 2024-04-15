//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Classic thread-safe queue with non-hanging facilities
//
//  * * * * * *
//    ^     ^
//    NRel  (NRel + NCur) % BufSize
//
//----------------------------------------------------------------------------

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

namespace {

constexpr int NTASKS = 1000;
constexpr int BUFSZ = 100;

template <typename T> class ts_queue {
  std::vector<T> Buffer; // fixed-size queue
  int NCur = 0;
  int NRel = 0; // start position
  bool Done = false;
  mutable std::mutex Mut;
  std::condition_variable CondCons, CondProd;

  // this interface cannot safely be public
  bool full() const { return NCur >= static_cast<int>(Buffer.size()); }
  bool empty() const { return NCur <= 0; }
  bool done() const { return Done; }

public:
  ts_queue(int BufSize) : Buffer(BufSize) {
    // prevent integer overflow cases
    if (BufSize > (1 << 30))
      throw std::runtime_error("unsupported buffer size");
  }

  void push(T Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondProd.wait(Lk, [this] { return !full(); });

    // exception safety
    Buffer[(NRel + NCur) % Buffer.size()] = Data;
    std::clog << "+";
    ++NCur;
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
    --NCur;
    std::clog << "-";
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
    return (NCur <= 0) && Done;
  }
};

int NTasks;
std::mutex TaskMut;

std::vector<int> Consumed;
std::mutex ConsMut;

void produce(ts_queue<int> &Q) {
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
    Q.push(N);
  }
}

void consume(ts_queue<int> &Q) {
  for (;;) {
    // critical section
    {
      std::lock_guard<std::mutex> Lk{TaskMut};
      if (NTasks < 0 && Q.is_empty_and_done())
        break;
    }

    int N;
    bool Succ = Q.wait_and_pop(N);

    // record what was consumed
    if (Succ) {
      std::lock_guard<std::mutex> Lk{ConsMut};
      Consumed.push_back(N);
    }
  }
}

} // namespace

void check_consumed() {
  // Hi MIPT! -- YouTube user
  std::set<int> Consumed_set(Consumed.begin(), Consumed.end());
  EXPECT_EQ(Consumed_set.size(), Consumed.size());
}

int check_env() {
  if (std::getenv("VERBOSE") == nullptr) {
    std::clog.setstate(std::ios_base::eofbit);
  }
  return 0;
}

int unused = check_env();

TEST(queue, classic_queue_1_1) {
  NTasks = NTASKS;
  ts_queue<int> Q(BUFSZ);
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{consume, std::ref(Q)};

  t1.join();
  Q.wake_and_done();
  t2.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1); // [0 .. NTASKS] inclusive
  EXPECT_EQ(NTasks, -1);
  check_consumed();
}

TEST(queue, classic_queue_1_2) {
  NTasks = NTASKS;
  ts_queue<int> Q(BUFSZ);
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{consume, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};

  t1.join();
  Q.wake_and_done();
  t2.join();
  t3.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1); // [0 .. NTASKS] inclusive
  EXPECT_EQ(NTasks, -1);
  check_consumed();
}

TEST(queue, classic_queue_2_1) {
  NTasks = NTASKS;
  ts_queue<int> Q(BUFSZ);
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{produce, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};

  t1.join();
  t2.join();
  Q.wake_and_done();
  t3.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1); // [0 .. NTASKS] inclusive
  EXPECT_EQ(NTasks, -1);
  check_consumed();
}

TEST(queue, classic_queue_2_2) {
  NTasks = NTASKS;
  ts_queue<int> Q(BUFSZ);
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{produce, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};
  std::thread t4{consume, std::ref(Q)};

  t1.join();
  t2.join();
  Q.wake_and_done();
  t3.join();
  t4.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1); // [0 .. NTASKS] inclusive
  EXPECT_EQ(NTasks, -1);
  check_consumed();
}
