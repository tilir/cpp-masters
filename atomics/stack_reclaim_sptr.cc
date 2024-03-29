//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Lock-free node-based stack with atomic shared pointer reclaim
//
// try also NTASKS = 100
//
// g++-12 -std=c++20 -g stack_reclaim_sptr.cc -lgtest -lgtest_main -pthread
// -latomic Also try with -fsanitize=thread
//
//----------------------------------------------------------------------------

#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

namespace {

#ifndef NTASKS
#define NTASKS 1000
#endif

std::vector<int> Consumed;
std::mutex ConsMut;

template <typename T> class lf_stack {
  struct Node {
    std::shared_ptr<Node> Next;
    T Data;
    Node(T Data) : Data(std::move(Data)) {}
  };

  std::atomic<std::shared_ptr<Node>> Head = nullptr;

  struct Ref {
    std::shared_ptr<Node> P;
    T &operator*() { return P->Data; }
    T *operator->() { return &P->Data; }
  };

public:
  void push(T Data) {
    auto New = std::make_shared<Node>(std::move(Data));
    New->Next = Head.load();
    while (!Head.compare_exchange_weak(New->Next, New))
      std::this_thread::yield();
  }

  std::shared_ptr<T> pop() {
    auto Old = Head.load();
    do {
      if (!Old)
        return nullptr;
      std::this_thread::yield();
    } while (!Head.compare_exchange_weak(Old, Old->Next));
    if (!Old)
      return nullptr;

    // aliasing ctor
    return std::shared_ptr<T>{Old, &Old->Data};
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
    auto SData = Q.pop();
    if (SData) {
      // record what was consumed
      std::lock_guard<std::mutex> Lk{ConsMut};
      Consumed.push_back(*SData);
    }
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
