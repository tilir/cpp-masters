//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Solution to distance in all its generality
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>
#include <iterator>

namespace {

// -------------------------------- stdlib-like part

template <class T, class U>
concept DerivedFrom = std::is_base_of<U, T>::value;

template <typename I>
concept Iterator = requires() {
  typename I::value_type;
  typename I::difference_type;
  typename I::reference;
  typename I::pointer;
};

template <typename I>
concept InputIterator = Iterator<I> && requires {
  typename I::iterator_category;
} && DerivedFrom<typename I::iterator_category, std::input_iterator_tag>;

template <typename I>
concept Incrementable = requires(I it) {
  ++it;
  it++;
};

template <typename I>
concept ForwardIterator = InputIterator<I> && Incrementable<I> &&
    DerivedFrom<typename I::iterator_category, std::forward_iterator_tag>;

template <typename I>
concept Decrementable = requires(I it) {
  --it;
  it--;
};

template <typename I>
concept BidirectionalIterator = ForwardIterator<I> && Decrementable<I> &&
    DerivedFrom<typename I::iterator_category, std::bidirectional_iterator_tag>;

template <typename I>
concept RandomAccess = requires(I it, typename I::difference_type n) {
  it + n;
  it - n;
};

template <typename I>
concept RandomAccessIterator = BidirectionalIterator<I> && RandomAccess<I> &&
    DerivedFrom<typename I::iterator_category, std::random_access_iterator_tag>;

template <InputIterator Iter>
typename Iter::difference_type my_distance(Iter first, Iter last) {
  typename Iter::difference_type n = 0;
  while (first != last) {
    ++first;
    ++n;
  }
  return n;
}

template <RandomAccessIterator Iter>
typename Iter::difference_type my_distance(Iter first, Iter last) {
  return last - first;
}

// --------------------------------

class junk_iter_t {
  int *state_;

public:
  using iterator_type = junk_iter_t;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = int;
  using difference_type = int;
  using pointer = value_type *;
  using reference = value_type &;

public:
  junk_iter_t(int *state = nullptr) : state_(state) {}
  junk_iter_t(const junk_iter_t &) = default;
  junk_iter_t(junk_iter_t &&) = default;
  junk_iter_t &operator=(const junk_iter_t &) = default;
  junk_iter_t &operator=(junk_iter_t &&) = default;

public:
  junk_iter_t &operator++() {
    ++state_;
    return *this;
  }

  junk_iter_t operator++(int) {
    auto temp(*this);
    ++(*this);
    return temp;
  }

  junk_iter_t &operator+=(int n) {
    for (int i = 0; i < n; ++i)
      ++(*this);
    return *this;
  }

  junk_iter_t &operator--() {
    --state_;
    return *this;
  }

  junk_iter_t operator--(int) {
    auto temp(*this);
    --(*this);
    return temp;
  }

  junk_iter_t &operator-=(int n) {
    for (int i = 0; i < n; ++i)
      --(*this);
    return *this;
  }

  value_type &operator*() const { return *state_; }
  value_type *operator->() const { return state_; }

  bool equals(const junk_iter_t &lhs) const { return state_ == lhs.state_; }
};

static inline bool operator==(junk_iter_t lhs, junk_iter_t rhs) {
  return lhs.equals(rhs);
}

static inline bool operator!=(junk_iter_t lhs, junk_iter_t rhs) {
  return !lhs.equals(rhs);
}

static inline junk_iter_t operator+(junk_iter_t it, int n) {
  it += n;
  return it;
}

static inline junk_iter_t operator-(junk_iter_t it, int n) {
  it -= n;
  return it;
}

} // namespace

TEST(concepts, distworking) {
  int arr[10];
  junk_iter_t fst(arr), snd(arr + 3);
  auto dist = my_distance(fst, snd);
  EXPECT_EQ(dist, 3);
}