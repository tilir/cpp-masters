//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Resumable test
//
//----------------------------------------------------------------------------

#include <coroutine>

#include "gtest/gtest.h"

namespace {

struct resumable {
  struct promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };

  using coro_handle = std::coroutine_handle<promise_type>;
  resumable(coro_handle handle) : handle_(handle) { assert(handle); }
  resumable(resumable &) = delete;
  resumable(resumable &&rhs) : handle_(rhs.handle_) { rhs.handle_ = nullptr; }
  bool resume() {
    if (!handle_.done())
      handle_.resume();
    return !handle_.done();
  }
  ~resumable() {
    if (handle_)
      handle_.destroy();
  }

private:
  coro_handle handle_;
};

} // namespace

namespace {

int G = 0;

resumable foo() {
  G += 1;
  co_await std::suspend_always();
  G += 1;
}

} // namespace

TEST(coroutines, resumable) {
  EXPECT_EQ(G, 0);
  auto t = foo();
  EXPECT_EQ(G, 0);
  t.resume();
  EXPECT_EQ(G, 1);
  t.resume();
  EXPECT_EQ(G, 2);
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif