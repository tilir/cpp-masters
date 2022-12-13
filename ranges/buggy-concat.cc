//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// buggy concat view
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;
namespace views = ranges::views;

template <ranges::view V1, ranges::view V2>
class concat_view : public ranges::view_interface<concat_view<V1, V2>> {
  V1 fst_;
  V2 snd_;

public:
  class iterator;
  class sentinel;

public:
  concat_view() {}
  concat_view(V1 fst, V2 snd) : fst_(std::move(fst)), snd_(std::move(snd)) {}
  auto begin();
  auto end();
};

template <ranges::view V1, ranges::view V2>
struct concat_view<V1, V2>::sentinel {
  ranges::sentinel_t<V2> s_;
};

template <ranges::view V1, ranges::view V2>
class concat_view<V1, V2>::iterator {
  concat_view<V1, V2> *parent_ = nullptr;
  ranges::iterator_t<V1> ifst_;
  ranges::iterator_t<V2> isnd_;
  bool first_done_ = false;

public:
  using iterator_category = std::forward_iterator_tag;
  using reference = ranges::range_reference_t<V1>;
  using value_type = ranges::range_value_t<V1>;
  using difference_type = std::common_type<ranges::range_difference_t<V1>,
                                           ranges::range_difference_t<V2>>;

  iterator() = default;
  iterator(concat_view<V1, V2> &parent)
      : parent_(&parent), ifst_(ranges::begin(parent.fst_)),
        isnd_(ranges::begin(parent.snd_)) {}

  value_type operator*() const { return !first_done_ ? *ifst_ : *isnd_; }

  iterator &operator++() {
    if (!first_done_) {
      ++ifst_;
      if (ifst_ == ranges::end(parent_->fst_))
        first_done_ = true;
    } else
      ++isnd_;

    return *this;
  }

  iterator operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  bool operator==(const iterator &) const = default;
  bool operator==(const concat_view<V1, V2>::sentinel &s) const {
    return first_done_ && isnd_ == s.s_;
  }
};

template <ranges::view V1, ranges::view V2> auto concat_view<V1, V2>::begin() {
  return concat_view<V1, V2>::iterator{*this};
}

template <ranges::view V1, ranges::view V2> auto concat_view<V1, V2>::end() {
  return concat_view<V1, V2>::sentinel{ranges::end(snd_)};
}

template <ranges::view V1, ranges::view V2> auto concat(V1 &&v1, V2 &&v2) {
  return concat_view<V1, V2>{std::forward<V1>(v1), std::forward<V2>(v2)};
};

bool is_prime(int n) {
  for (int i = 2; i * i < n; ++i)
    if ((n % i) == 0)
      return false;
  return true;
}

int main() {
  std::vector<int> v1 = {-1, -2, -3};
  std::vector<int> v2 = {-3, -2, -1};
  auto chain = concat(views::all(v1), views::all(v2));
  for (auto i : chain)
    std::cout << i << " ";
  std::cout << std::endl;

  auto first_two = concat(views::single(2), views::single(3));
  for (auto i : first_two)
    std::cout << i << " ";
  std::cout << std::endl;

  auto modulo_six = [](auto x) { return ((x % 6) == 1) || ((x % 6) == 5); };
  auto others =
      views::iota(5) | views::filter(modulo_six) | views::filter(is_prime);
  auto primes = concat(first_two, others);
  for (auto i : ranges::take_view(primes, 20))
    std::cout << i << " ";
  std::cout << std::endl;
}