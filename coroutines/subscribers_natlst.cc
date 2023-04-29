//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Subscribers with natural list to be made atomic
//
//----------------------------------------------------------------------------

#include <coroutine>
#include <list>

#include "gtest/gtest.h"

namespace {

struct resumable_no_own {
  struct promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_never(); }

    // this one is critical: no suspend on final suspend
    // effectively means "destroy your frame"
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

  // natural list: this is top and we will amend to top
  awaiter *top_ = nullptr;
  bool set_;

  struct awaiter {
    evt_awaiter_t &event_;
    awaiter *next_;
    coro_t coro_ = nullptr;
    awaiter(evt_awaiter_t &event) noexcept : event_(event) {}

    bool await_ready() const noexcept { return event_.is_set(); }

    void await_suspend(coro_t c) noexcept {
      coro_ = c;
      event_.push_awaiter(this);
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
  bool is_set() const noexcept { return set_; }
  void push_awaiter(awaiter *a) {
    awaiter *oldtop = top_;
    top_ = a;
    a->next_ = oldtop;
  }

  awaiter operator co_await() noexcept { return awaiter{*this}; }

  void set() noexcept {
    set_ = true;
    auto cur_ = top_;
    top_ = nullptr;
    while (cur_) {
      cur_->coro_.resume();
      cur_ = cur_->next_;
    }
  }

  void reset() noexcept { set_ = false; }
};

} // namespace

namespace {

int G = 0, C = 0;
evt_awaiter_t Evt;

void producer() {
  G += 42;
  Evt.set();
}

resumable_no_own consumer1() {
  co_await Evt;
  EXPECT_GT(G, 0); // 42 or 84 depending on
  C += 1;
}

resumable_no_own consumer2() {
  EXPECT_EQ(G, 0);
  co_await Evt;
  EXPECT_EQ(G, 42);
  C += 1;
}

resumable_no_own consumer3() {
  EXPECT_EQ(G, 0);
  co_await Evt;
  EXPECT_EQ(G, 42);
  co_await Evt;
  EXPECT_EQ(G, 84);
  C += 1;
}

} // namespace

TEST(coroutines, subscribers_natlst) {
  consumer1();
  consumer2();
  consumer3();
  producer();
  consumer1();
  producer();
  EXPECT_EQ(C, 4);
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif