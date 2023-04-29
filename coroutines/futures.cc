//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// McNellis futures implementation for C++20
//
//----------------------------------------------------------------------------

#include <coroutine>
#include <future>

#include "gtest/gtest.h"

template <> struct std::coroutine_traits<std::future<int>> {
  struct promise_type : std::promise<int> {
    std::future<int> get_return_object() { return this->get_future(); }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_value(int value) { this->set_value(value); }
    void unhandled_exception() {
      this->set_exception(std::current_exception());
    }
  };
};

namespace {

auto operator co_await(std::future<int> future) {
  struct awaiter : std::future<int> {
    bool await_ready() { return false; } // suspend always
    void await_suspend(std::coroutine_handle<> handle) {
      std::thread([this, handle]() {
        this->wait();
        handle.resume();
      }).detach();
    }
    int await_resume() { return this->get(); }
  };
  return awaiter{std::move(future)};
}

} // namespace

namespace {

std::future<int> compute_value() {
  int result = co_await std::async([] { return 42; });
  co_return result;
}

} // namespace

TEST(coroutines, futures) {
  auto F = compute_value();
  auto V = F.get();
  EXPECT_EQ(V, 42);
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif