//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Lock-free bounded queue
//
// passes with sanitizer and everything
//
//----------------------------------------------------------------------------

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

namespace {

#ifndef NTASKS
#define NTASKS 1000
#endif

#ifndef BUFSIZE
#define BUFSIZE 128
#endif

std::vector<int> Consumed;
std::mutex ConsMut;
std::mutex LogMut;

template <typename T> class lf_queue {
  // using unsigned to allow legal wrap around
  struct CellTy {
    std::atomic<unsigned> Sequence;
    T Data;
  };

  std::vector<CellTy> Buffer;
  unsigned BufferMask;
  std::atomic<unsigned> EnqueuePos, DequeuePos;

public:
  lf_queue(unsigned BufSize) : Buffer(BufSize), BufferMask(BufSize - 1) {
    if (BufSize > (1 << 30))
      throw std::runtime_error("buffer size too large");

    if (BufSize < 2)
      throw std::runtime_error("buffer size too small");

    if ((BufSize & (BufSize - 1)) != 0)
      throw std::runtime_error("buffer size is not power of 2");

    for (unsigned i = 0; i != BufSize; ++i)
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
      auto Seq = Cell->Sequence.load(std::memory_order_acquire);
      auto Diff = static_cast<int>(Seq) - static_cast<int>(Pos);

#ifdef LOG
      {
        std::lock_guard<std::mutex> Lk(LogMut);
        std::cout << "push: ";
        std::cout << Pos << " " << Cell << " " << Seq << " " << Diff
                  << std::endl;
      }
#endif

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

#ifdef LOG
      {
        std::lock_guard<std::mutex> Lk(LogMut);
        std::cout << "pop: ";
        std::cout << Pos << " " << Cell << " " << Seq << " " << Diff
                  << std::endl;
      }
#endif

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

void produce(lf_queue<int> &Q) {
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
    while (!Q.push(N))
      std::this_thread::yield();
  }
}

void consume(lf_queue<int> &Q) {
  for (;;) {
    int N = NTasks.load();
    if (N < 0 && Q.is_empty())
      break;
    bool Succ = Q.pop(N);
    if (Succ) {
      // record what was consumed
      std::lock_guard<std::mutex> Lk{ConsMut};
      Consumed.push_back(N);
    }
  }
}

} // namespace

TEST(atomics, bounded_lockfree_queue_1_1) {
  NTasks = NTASKS;
  lf_queue<int> Q(BUFSIZE);
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

TEST(atomics, bounded_lockfree_queue_1_2) {
  NTasks = NTASKS;
  lf_queue<int> Q(BUFSIZE);
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

TEST(atomics, bounded_lockfree_queue_2_1) {
  NTasks = NTASKS;
  lf_queue<int> Q(BUFSIZE);
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

TEST(atomics, bounded_lockfree_queue_2_2) {
  NTasks = NTASKS;
  lf_queue<int> Q(BUFSIZE);
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

TEST(atomics, bounded_lockfree_queue_4_4) {
  NTasks = NTASKS;
  lf_queue<int> Q(BUFSIZE);
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
