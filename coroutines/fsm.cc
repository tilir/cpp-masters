//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Test for finite state machines
//
//----------------------------------------------------------------------------

#include <cassert>
#include <coroutine>
#include <unordered_map>
#include <vector>

#include "gtest/gtest.h"

namespace {

template <typename T> struct generator {
  struct promise_type {
    T current_value;
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    auto yield_value(T value) {
      current_value = value;
      return std::suspend_always{};
    }
  };

  using coro_handle = std::coroutine_handle<promise_type>;
  bool move_next() {
    return handle_ ? (handle_.resume(), !handle_.done()) : false;
  }
  T current_value() const { return handle_.promise().current_value; }
  generator(coro_handle h) : handle_(h) {}
  generator(generator const &) = delete;
  generator(generator &&rhs) : handle_(rhs.handle_) { rhs.handle_ = nullptr; }
  ~generator() {
    if (handle_)
      handle_.destroy();
  }

private:
  coro_handle handle_;
};

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

struct resumable_noinc {
  struct promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };

  using coro_handle = std::coroutine_handle<promise_type>;
  resumable_noinc(coro_handle handle) : handle_(handle) { assert(handle); }
  resumable_noinc(resumable_noinc &) = delete;
  resumable_noinc(resumable_noinc &&rhs) : handle_(rhs.handle_) {
    rhs.handle_ = nullptr;
  }
  bool resume() {
    if (!handle_.done())
      handle_.resume();
    return !handle_.done();
  }
  ~resumable_noinc() {
    if (handle_)
      handle_.destroy();
  }
  coro_handle handle() {
    coro_handle h = handle_;
    handle_ = nullptr;
    return h;
  }

private:
  coro_handle handle_;
};

using coro_t = std::coroutine_handle<>;

template <typename State, typename Sym> class state_machine {
  State current_;
  std::unordered_map<State, coro_t> states_;
  generator<Sym> gen_;

public:
  state_machine(generator<Sym> &&g) : gen_{std::move(g)} {}

  void run(State initial) {
    current_ = initial;
    states_[initial].resume();
  }

  template <typename F> void add_state(State x, F stf) {
    states_[x] = stf(*this).handle();
  }

  coro_t operator[](State s) { return states_[s]; }

  State current() const { return current_; }

  template <typename F> auto get_awaiter(F transition);

  Sym genval() const { return gen_.current_value(); }
  void gennext() { gen_.move_next(); }
};

template <typename F, typename Sym, typename SM> struct stm_awaiter : public F {
  SM &stm_;
  stm_awaiter(F f, SM &stm) : F{f}, stm_{stm} {}
  bool await_ready() const noexcept { return false; }
  coro_t await_suspend(coro_t) noexcept {
    stm_.gennext();
    auto sym = stm_.genval();
    auto newstate = F::operator()(sym);
    return stm_[newstate];
  }
  bool await_resume() noexcept { return (stm_.genval() == Sym::Term); }
};

template <typename State, typename Sym>
template <typename F>
auto state_machine<State, Sym>::get_awaiter(F transition) {
  return stm_awaiter<F, Sym, decltype(*this)>(transition, *this);
}

} // namespace

namespace {

enum class State : char { A, B };
enum class Sym : char { A, B, Term };

using stm_t = state_machine<State, Sym>;

generator<Sym> input_seq(std::string s) {
  for (char c : s) {
    switch (std::tolower(c)) {
    case 'a':
      co_yield Sym::A;
      break;
    case 'b':
      co_yield Sym::B;
      break;
    default:
      co_yield Sym::Term;
      break;
    }
  }

  for (;;)
    co_yield Sym::Term;
}

resumable_noinc StateA(stm_t &stm) {
  auto transition = [](auto sym) {
    if (sym == Sym::B)
      return State::B;
    return State::A;
  };

  for (;;) {
    std::cout << "State A" << std::endl;
    bool finish = co_await stm.get_awaiter(transition);
    if (finish)
      break;
  }
}

resumable_noinc StateB(stm_t &stm) {
  auto transition = [](auto sym) {
    if (sym == Sym::A)
      return State::A;
    return State::B;
  };

  for (;;) {
    std::cout << "State B" << std::endl;
    bool finish = co_await stm.get_awaiter(transition);
    if (finish)
      break;
  }
}

} // namespace

TEST(coroutines, fsm) {
  auto gen = input_seq("aaabbaba");
  stm_t stm{std::move(gen)};
  stm.add_state(State::A, StateA);
  stm.add_state(State::B, StateB);
  stm.run(State::A);
  auto Cur = stm.current();
  EXPECT_EQ(Cur, State::A);
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif