//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Subscribers for multithreads -- bad attempt
//
//----------------------------------------------------------------------------

#include <coroutine>
#include <list>
#include <mutex>

#include "gtest/gtest.h"

namespace {

struct resumable_no_own {
  struct promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_never(); }
    auto final_suspend() noexcept { return std::suspend_never(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };

  using coro_handle = std::coroutine_handle<promise_type>;
  resumable_no_own(coro_handle handle) {}
  resumable_no_own(resumable_no_own &) {}
  resumable_no_own(resumable_no_own &&rhs) {}
};

using coro_t = std::coroutine_handle<>;

class evt_awaiter_t {
  struct awaiter;

  mutable std::mutex m_;
  std::list<awaiter> lst_;
  bool set_;

  struct awaiter {
    evt_awaiter_t &event_;
    coro_t coro_ = nullptr;
    awaiter(evt_awaiter_t &event) noexcept : event_(event) {}

    bool await_ready() const noexcept { return event_.is_set(); }

    void await_suspend(coro_t c) noexcept {
      coro_ = c;
      event_.push_awaiter(*this);
    }

    void await_resume() noexcept { event_.reset(); }
  };

public:
  evt_awaiter_t(bool set = false) : set_{set} {}
  evt_awaiter_t(const evt_awaiter_t &) = delete;
  evt_awaiter_t &operator=(const evt_awaiter_t &) = delete;
  evt_awaiter_t(evt_awaiter_t &&) = delete;
  evt_awaiter_t &operator=(evt_awaiter_t &&) = delete;

public:
  bool is_set() const noexcept {
    std::lock_guard<std::mutex> L(m_);
    return set_;
  }
  void push_awaiter(awaiter a) {
    std::lock_guard<std::mutex> L(m_);
    lst_.push_back(a);
  }

  awaiter operator co_await() noexcept { return awaiter{*this}; }

  void set() noexcept {
    std::list<awaiter> toresume;
    {
      std::lock_guard<std::mutex> L(m_);
      set_ = true;
      toresume.splice(toresume.begin(), lst_);
    }
    for (auto s : toresume)
      s.coro_.resume();
  }

  void reset() noexcept { set_ = false; }
};

} // namespace

namespace {

std::mutex M;
int G = 0, C = 0;
evt_awaiter_t Evt;
std::atomic<int> L = 3;

void producer() {
  {
    std::lock_guard<std::mutex> L(M);
    G += 42;
  }

  Evt.set();
}

resumable_no_own consumer() {
  // this is fragile and errorneous. Figure out how to do better?
  L -= 1;
  co_await Evt;

  std::lock_guard<std::mutex> L(M);
  EXPECT_GT(G, 0);
  C += 1;
}

} // namespace

TEST(coroutines, subscribers_mt) {
  std::thread t1(consumer);
  std::thread t2(consumer);
  std::thread t3(consumer);

  // this is fragile and errorneous. Figure out how to do better?
  while (L > 0)
    std::this_thread::yield();

  producer();
  t1.join();
  t2.join();
  t3.join();
  EXPECT_EQ(C, 3);
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif