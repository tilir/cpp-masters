//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Test for cancellables
//
//----------------------------------------------------------------------------

#include <coroutine>

#include "gtest/gtest.h"

namespace {

struct suspend_tunable {
  bool tune_;
  suspend_tunable(bool tune = true) : tune_(tune) {}
  bool await_ready() const noexcept { return tune_; }
  void await_suspend(std::coroutine_handle<>) const noexcept {}
  void await_resume() const noexcept {}
};

struct resumable_cancelable {
  struct promise_type {
    bool is_cancelled = false;
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }

    auto await_transform(std::suspend_always) {
      if (is_cancelled)
        return suspend_tunable{true};
      return suspend_tunable{false};
    }
  };

  using coro_handle = std::coroutine_handle<promise_type>;
  resumable_cancelable(coro_handle handle) : handle_(handle) { assert(handle); }
  resumable_cancelable(resumable_cancelable &) = delete;
  resumable_cancelable(resumable_cancelable &&rhs) : handle_(rhs.handle_) {
    rhs.handle_ = nullptr;
  }
  void cancel() {
    if (handle_.done())
      return;
    handle_.promise().is_cancelled = true;
    handle_.resume();
  }
  bool resume() {
    if (!handle_.done())
      handle_.resume();
    return !handle_.done();
  }
  ~resumable_cancelable() {
    if (handle_)
      handle_.destroy();
  }

private:
  coro_handle handle_;
};

} // namespace

namespace {

int G = 0;

resumable_cancelable foo() {
  for (int i = 0; i < 10; ++i) {
    G += 1;
    co_await std::suspend_always{};
  }
}

} // namespace

TEST(coroutines, cancellable) {
  auto f = foo();
  EXPECT_EQ(G, 0);
  f.resume();
  EXPECT_EQ(G, 1);
  f.resume();
  EXPECT_EQ(G, 2);
  f.resume();
  EXPECT_EQ(G, 3);
  f.cancel();
  EXPECT_EQ(G, 10);
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif