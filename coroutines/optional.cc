//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Improving optional to Maybe
//
//----------------------------------------------------------------------------

#include <cassert>
#include <coroutine>
#include <unordered_map>
#include <vector>

#include "gtest/gtest.h"

namespace {

template <typename T> class resume_optional {
  struct optional_awaiter {
    std::optional<T> opt_;

    optional_awaiter(std::optional<T> opt) : opt_{opt} {}

    bool await_ready() { return opt_.has_value(); }
    auto await_resume() { return opt_.value(); }
    void await_suspend(std::coroutine_handle<>) {}
  };

public:
  struct promise_type {
    std::optional<T> opt_;
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_never{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    optional_awaiter await_transform(std::optional<T> value) {
      return optional_awaiter{value};
    }
    void return_value(T value) { opt_ = value; }
    void unhandled_exception() { std::terminate(); }
  };

  using coro_handle = std::coroutine_handle<promise_type>;
  resume_optional(coro_handle handle) : handle_(handle) { assert(handle); }
  resume_optional(resume_optional &) = delete;
  resume_optional(resume_optional &&rhs) : handle_(rhs.handle_) {
    rhs.handle_ = nullptr;
  }

  std::optional<T> current_value() const {
    if (handle_)
      return handle_.promise().opt_;
    return std::nullopt;
  }

  ~resume_optional() {
    if (handle_)
      handle_.destroy();
  }

private:
  coro_handle handle_;
};

} // namespace

namespace {

std::optional<int> five() { return 5; }

std::optional<int> six() { return 6; }

std::optional<int> empty() { return std::nullopt; }

resume_optional<int> sum() {
  auto a = co_await five(); // 5 or return to caller with nullopt
  auto b = co_await six();  // 6 or return to caller with nullopt

  co_return a + b; // return sum
}

resume_optional<int> sum2() {
  auto a = co_await five();
  auto b = co_await empty();

  co_return a + b;
}

std::ostream &operator<<(std::ostream &os, const std::optional<int> &opt) {
  if (!opt)
    os << "(empty)";
  else
    os << opt.value();
  return os;
}

} // namespace

TEST(coroutines, optional) {
  auto Opt = sum();
  auto Val = Opt.current_value();
  EXPECT_EQ(Val, 11);

  auto Opt2 = sum2();
  auto Val2 = Opt2.current_value();
  EXPECT_EQ(Val2, std::nullopt);
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif