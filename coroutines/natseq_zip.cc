//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Range-based generation of two ranges at a time
//
//----------------------------------------------------------------------------

#include <coroutine>
#include <future>

#include "gtest/gtest.h"

namespace {

template <typename T> struct range_gen {
  struct promise_type {
    const T *value = nullptr;
#ifdef BUG
    auto get_return_object() { return *this; }
#else
    auto get_return_object() { return coro_handle::from_promise(*this); }
#endif
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    std::suspend_always yield_value(const T &val) {
      value = std::addressof(val);
      return {};
    }
  };

  using coro_handle = std::coroutine_handle<promise_type>;

  struct iterator {
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    coro_handle handle;
    iterator(coro_handle h) : handle(h) {}

    iterator &operator++() {
      handle.resume();
      if (handle.done())
        handle = nullptr;
      return *this;
    }

    T const &operator*() const { return *handle.promise().value; }
    T const *operator->() const { return handle.promise().value; }
    auto operator<=>(const iterator &rhs) const noexcept = default;
  };

  range_gen(promise_type &promise)
      : handle_(coro_handle::from_promise(promise)) {}
  range_gen(coro_handle h) : handle_(h) {}
  range_gen(const range_gen &) = delete;
  range_gen &operator=(const range_gen &other) = delete;
  range_gen(range_gen &&rhs) : handle_(rhs.handle_) { rhs.handle_ = nullptr; }
  range_gen &operator=(range_gen &&rhs) {
    if (this != &rhs) {
      handle_ = rhs.handle;
      rhs.handle = nullptr;
    }
    return *this;
  }

  ~range_gen() {
    if (handle_)
      handle_.destroy();
  }

  iterator begin() {
    if (!handle_)
      return iterator{nullptr};
    handle_.resume();
    if (handle_.done())
      return iterator{nullptr};
    return handle_;
  }

  iterator end() { return iterator{nullptr}; }

private:
  coro_handle handle_;
};

} // namespace

namespace {

template <typename T, typename U>
range_gen<std::pair<T, U>> zip(range_gen<T> t, range_gen<U> u) {
  auto itt = t.begin(), itte = t.end();
  auto itu = u.begin(), itue = u.end();
  for (; itt != itte && itu != itue; ++itt, ++itu)
    co_yield std::make_pair(*itt, *itu);
}

template <typename T> range_gen<T> sequence(T start, T fin, T step) {
  for (T num = start; num < fin; num += step)
    co_yield num;
}

} // namespace

TEST(coroutines, natseq_zip) {
  int IS = 25;
  int JS = 5;
  for (auto [I, J] : zip(sequence(25, 40, 3), sequence(5, 20, 3))) {
    EXPECT_EQ(I, IS);
    EXPECT_EQ(J, JS);
    IS += 3;
    JS += 3;
  }
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif