//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Classic thread-safe stack with non-hanging facilities
//
//----------------------------------------------------------------------------

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

namespace {

constexpr int NTASKS = 100;
constexpr int BUFSZ = 10;

std::vector<int> Consumed;
std::mutex ConsMut;

template <typename T> class ts_stack {
  std::vector<T> Buffer; // fixed-size stack
  int NCur = -1;
  bool Done = false;
  mutable std::mutex Mut;
  std::condition_variable CondCons, CondProd;

  // this interface cannot safely be public
  bool full() const { return NCur >= static_cast<int>(Buffer.size() - 1); }
  bool empty() const { return NCur < 0; }
  bool done() const { return Done; }

public:
  ts_stack(int BufSize) : Buffer(BufSize) {}

  void push(T Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondProd.wait(Lk, [this] { return !full(); });
    NCur += 1;
    Buffer[NCur] = Data;
    CondCons.notify_one();
  }

  bool wait_and_pop(T &Data) {
    std::unique_lock<std::mutex> Lk{Mut};
    CondCons.wait(Lk, [this] { return !empty() || done(); });
    if (empty())
      return false;
    Data = Buffer[NCur];
    NCur -= 1;
    CondProd.notify_one();
    return true;
  }

  void wake_and_done() {
    std::unique_lock<std::mutex> Lk{Mut};
    Done = true;
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

void produce(ts_stack<int> &Q) {
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

void consume(ts_stack<int> &Q) {
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

TEST(stack, classic_stack_1_1) {
  NTasks = NTASKS;
  ts_stack<int> Q(BUFSZ);
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{consume, std::ref(Q)};

  t1.join();

  // after all producers join
  Q.wake_and_done();

  t2.join();

#if 0
  std::cout << "Cons: " << std::endl;
  std::sort(Consumed.begin(), Consumed.end());
  int Cnt = 0;
  for (auto &&C : Consumed) {    
    std::cout << C << " ";
    if ((Cnt++ % 10) == 0)
      std::cout << std::endl;
  }
  std::cout << std::endl;
#endif

  EXPECT_EQ(Consumed.size(), NTASKS + 1); // [0 .. NTASKS] inclusive
  EXPECT_EQ(NTasks, -1);
}

TEST(stack, classic_stack_1_2) {
  NTasks = NTASKS;
  ts_stack<int> Q(BUFSZ);
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{consume, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};

  t1.join();

  // after all producers join
  Q.wake_and_done();

  t2.join();
  t3.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1);
  EXPECT_EQ(NTasks, -1);
}

TEST(stack, classic_stack_2_1) {
  NTasks = NTASKS;
  ts_stack<int> Q(BUFSZ);
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{produce, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};

  t1.join();
  t2.join();

  // after all producers join
  Q.wake_and_done();

  t3.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1);
  EXPECT_EQ(NTasks, -1);
}

TEST(stack, classic_stack_2_2) {
  NTasks = NTASKS;
  ts_stack<int> Q(BUFSZ);
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{produce, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};
  std::thread t4{consume, std::ref(Q)};

  t1.join();
  t2.join();

  // after all producers join
  Q.wake_and_done();

  t3.join();
  t4.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1);
  EXPECT_EQ(NTasks, -1);
}
