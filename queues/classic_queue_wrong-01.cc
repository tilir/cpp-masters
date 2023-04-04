//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Classic thread-safe queue with non-hanging facilities
// step 1: this is not queue at all
//
// g++-11 classic_queue_wrong-01.cc -lgtest -lgtest_main -pthread
//
//----------------------------------------------------------------------------

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "gtest/gtest.h"

namespace {

constexpr int NTASKS = 1000;
constexpr int BUFSZ = 100;

template <typename T> class ts_queue {
  std::vector<T> Buffer; // fixed-size queue
  int NCur = -1;
  bool Done = false;
  mutable std::mutex Mut;
  std::condition_variable CondCons, CondProd;

  // this interface cannot safely be public
  bool full() const { return NCur >= static_cast<int>(Buffer.size()); }
  bool empty() const { return NCur < 0; }
  bool done() const { return Done; }

public:
  ts_queue(int BufSize) : Buffer(BufSize) {}

  void push(T Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondProd.wait(Lk, [this] { return !full() || done(); });
    if (Done)
      return;
    NCur += 1;
    Buffer[NCur] = Data;
    CondCons.notify_one();
  }

  void wait_and_pop(T &Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondCons.wait(Lk, [this] { return !empty() || done(); });
    if (Done)
      return;
    Data = Buffer[NCur];
    NCur -= 1;
    CondProd.notify_one(); // wake up producer
  }

  void wake_and_done() {
    Done = true;
    CondCons.notify_all();
    CondProd.notify_all();
  }
};

int NTasks;
std::mutex TaskMut;

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
  Q.wake_and_done();
}

void consume(ts_queue<int> &Q) {
  for (;;) {
    int N;
    // critical section
    {
      std::lock_guard<std::mutex> Lk{TaskMut};
      if (NTasks < 0)
        break;
    }
    Q.wait_and_pop(N);
  }
  Q.wake_and_done();
}

} // namespace

TEST(queue, classic_queue_1_1) {
  NTasks = NTASKS;
  ts_queue<int> Q(BUFSZ);
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{consume, std::ref(Q)};

  t1.join();
  t2.join();

  EXPECT_EQ(NTasks, -1);
}

TEST(queue, classic_queue_1_2) {
  NTasks = NTASKS;
  ts_queue<int> Q(BUFSZ);
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{consume, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};

  t1.join();
  t2.join();
  t3.join();

  EXPECT_EQ(NTasks, -1);
}

TEST(queue, classic_queue_2_1) {
  NTasks = NTASKS;
  ts_queue<int> Q(BUFSZ);
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{produce, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};

  t1.join();
  t2.join();
  t3.join();

  EXPECT_EQ(NTasks, -1);
}

TEST(queue, classic_queue_2_2) {
  NTasks = NTASKS;
  ts_queue<int> Q(BUFSZ);
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{produce, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};
  std::thread t4{consume, std::ref(Q)};

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  EXPECT_EQ(NTasks, -1);
}
