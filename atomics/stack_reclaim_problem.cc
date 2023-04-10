//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Lock-free node-based stack with reclaim problems
//
// try also NTASKS = 100
//
// g++-11 -O2 stack_reclaim_problem.cc -lgtest -lgtest_main -pthread
// Also try with -fsanitize=thread
//
//----------------------------------------------------------------------------

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

namespace {

constexpr int NTASKS = 1000;

std::vector<int> Consumed;
std::mutex ConsMut;

template <typename T> class lf_stack {
  struct Node {
    Node *Next;
    T Data;
  };
  std::atomic<Node *> Head = nullptr;

public:
  void push(T Data) {
    auto New = new Node{Head.load(), std::move(Data)};
    while (!Head.compare_exchange_weak(New->Next, New))
      std::this_thread::yield();
  }

  void pop(T &Data) {
    Node *Old = Head.load();
    while (!Head.compare_exchange_weak(Old, Old->Next))
      std::this_thread::yield();
    Data = std::move(Old->Data);
    delete Old;
  }

  bool is_empty() const { return Head.load() == nullptr; }
};

std::atomic<int> NTasks;

void produce(lf_stack<int> &Q) {
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
    Q.push(N);
  }
}

void consume(lf_stack<int> &Q) {
  for (;;) {
    int N = NTasks.load();
    if (N < 0 && Q.is_empty())
      break;
    Q.pop(N);
    // record what was consumed
    std::lock_guard<std::mutex> Lk{ConsMut};
    Consumed.push_back(N);
  }
}

} // namespace

TEST(atomics, lockfree_stack_1_1) {
  NTasks = NTASKS;
  lf_stack<int> Q;
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{consume, std::ref(Q)};

  t1.join();
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

TEST(atomics, lockfree_stack_1_2) {
  NTasks = NTASKS;
  lf_stack<int> Q;
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{consume, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};

  t1.join();
  t2.join();
  t3.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1);
  EXPECT_EQ(NTasks, -1);
}

TEST(atomics, lockfree_stack_2_1) {
  NTasks = NTASKS;
  lf_stack<int> Q;
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{produce, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};

  t1.join();
  t2.join();
  t3.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1);
  EXPECT_EQ(NTasks, -1);
}

TEST(atomics, lockfree_stack_2_2) {
  NTasks = NTASKS;
  lf_stack<int> Q;
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{produce, std::ref(Q)};
  std::thread t3{consume, std::ref(Q)};
  std::thread t4{consume, std::ref(Q)};

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1);
  EXPECT_EQ(NTasks, -1);
}

TEST(atomics, lockfree_stack_4_4) {
  NTasks = NTASKS;
  lf_stack<int> Q;
  Consumed.clear();
  std::thread t1{produce, std::ref(Q)};
  std::thread t2{produce, std::ref(Q)};
  std::thread t3{produce, std::ref(Q)};
  std::thread t4{produce, std::ref(Q)};
  std::thread t5{consume, std::ref(Q)};
  std::thread t6{consume, std::ref(Q)};
  std::thread t7{consume, std::ref(Q)};
  std::thread t8{consume, std::ref(Q)};

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
  t8.join();

  EXPECT_EQ(Consumed.size(), NTASKS + 1);
  EXPECT_EQ(NTasks, -1);
}
