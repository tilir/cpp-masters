/* slist.h                  -*-C++-*-
 *
 *            Copyright 2017 Pablo Halpern.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef INCLUDED_SLIST_DOT_H
#define INCLUDED_SLIST_DOT_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <memory_resource>

using std::addressof;
using std::allocator_traits;
using std::byte;
using std::equal;
using std::forward;
using std::forward_iterator_tag;
using std::move;
using std::ptrdiff_t;
using std::swap;

using std::pmr::monotonic_buffer_resource;
using std::pmr::polymorphic_allocator;

namespace slist_details {

template <typename Tp> struct node;

template <typename Tp> struct node_base {
  node<Tp> *next_;

  node_base() : next_(nullptr) {}
  node_base(node_base &&rhs) : next_(rhs.next_) { rhs.next_ = nullptr; }
  node_base &operator=(node_base &&rhs) {
    if (&rhs == this)
      return *this;
    next_ = rhs.next_;
    rhs.next_ = nullptr;
    return *this;
  }
  node_base(const node_base &) = delete;
  node_base &operator=(const node_base &) = delete;
};

template <typename Tp> struct node : node_base<Tp> {
  union {
    Tp value_;
  };
};

template <typename Tp> struct const_iterator {
  using value_type = Tp;
  using pointer = Tp const *;
  using reference = Tp const &;
  using difference_type = ptrdiff_t;
  using iterator_category = forward_iterator_tag;

  reference operator*() const { return prev_->next_->value_; }
  pointer operator->() const { return addressof(prev_->next_->value_); }

  const_iterator &operator++() {
    prev_ = prev_->next_;
    return *this;
  }
  const_iterator operator++(int) {
    const_iterator tmp(*this);
    ++*this;
    return tmp;
  }

  bool operator==(const_iterator other) const { return prev_ == other.prev_; }
  bool operator!=(const_iterator other) const { return !operator==(other); }

  node_base<Tp> *prev_; // pointer to node before current element

  explicit const_iterator(const node_base<Tp> *prev)
      : prev_(const_cast<node_base<Tp> *>(prev)) {}
};

template <typename Tp> struct iterator : public const_iterator<Tp> {
  using Base = const_iterator<Tp>;

  using pointer = Tp *;
  using reference = Tp &;

  reference operator*() const { return this->prev_->next_->value_; }
  pointer operator->() const { return addressof(this->prev_->next_->value_); }

  iterator &operator++() {
    Base::operator++();
    return *this;
  }
  iterator operator++(int) {
    iterator tmp(*this);
    ++*this;
    return tmp;
  }

  explicit iterator(node_base<Tp> *prev) : const_iterator<Tp>(prev) {}
};

} // namespace slist_details

// Singly-linked list that supports the use of a polymorphic
// allocator.
template <typename Tp> class slist {
public:
  using value_type = Tp;
  using reference = value_type &;
  using const_reference = value_type const &;
  using difference_type = ptrdiff_t;
  using size_type = size_t;
  using allocator_type = polymorphic_allocator<byte>;
  using iterator = slist_details::iterator<Tp>;
  using const_iterator = slist_details::const_iterator<Tp>;
  using traits = allocator_traits<allocator_type>;

  slist(allocator_type a = {}) : head_(), ptail_(&head_), size_(0), alloc_(a) {}
  slist(const slist &other, allocator_type a = {});
  slist(slist &&other) noexcept;
  slist(slist &&other, allocator_type a);
  ~slist();

  slist &operator=(const slist &other);
  slist &operator=(slist &&other);
  void swap(slist &other) noexcept;

  size_t size() const noexcept { return size_; }
  bool empty() const noexcept { return 0 == size_; }

  iterator begin() { return iterator(&head_); }
  iterator end() { return iterator(ptail_); }
  const_iterator begin() const { return const_iterator(&head_); }
  const_iterator end() const { return const_iterator(ptail_); }
  const_iterator cbegin() const { return const_iterator(&head_); }
  const_iterator cend() const { return const_iterator(ptail_); }

  Tp &front() { return head_.next_->value_; }
  Tp const &front() const { return head_.next_->value_; }

  template <typename... Args> iterator emplace(iterator i, Args &&... args);
  template <typename... Args> void emplace_front(Args &&... args) {
    emplace(begin(), forward<Args>(args)...);
  }
  template <typename... Args> void emplace_back(Args &&... args) {
    emplace(end(), forward<Args>(args)...);
  }

  iterator insert(iterator i, const Tp &v) { return emplace(i, v); }
  void push_front(const Tp &v) { emplace(begin(), v); }
  void push_back(const Tp &v) { emplace(end(), v); }

  // Note: erasing elements invalidates iterators to the node
  // following the element being erased.
  iterator erase(iterator b, iterator e);
  iterator erase(iterator i) {
    iterator e = i;
    return erase(i, ++e);
  }
  void pop_front() { erase(begin()); }

  allocator_type get_allocator() const { return alloc_; }

private:
  using node_base = slist_details::node_base<Tp>;
  using node = slist_details::node<Tp>;

  node_base head_;
  node_base *ptail_;
  size_t size_;
  allocator_type alloc_;
};

namespace {
template <class Tp> bool operator==(const slist<Tp> &a, const slist<Tp> &b) {
  if (a.size() != b.size())
    return false;
  else
    return equal(a.begin(), a.end(), b.begin());
}

template <class Tp> bool operator!=(const slist<Tp> &a, const slist<Tp> &b) {
  return !(a == b);
}
} // namespace

///////////// Implementation ///////////////////

template <typename Tp>
slist<Tp>::slist(const slist<Tp> &other, allocator_type a) : slist<Tp>(a) {
  operator=(other);
}

template <typename Tp>
slist<Tp>::slist(slist<Tp> &&rhs) noexcept : slist<Tp>(rhs.get_allocator()) {
  head_ = move(rhs.head_);
  ptail_ = move(rhs.ptail_);
}

template <typename Tp>
slist<Tp>::slist(slist &&other, allocator_type a) : slist(a) {
  operator=(move(other));
}

template <typename Tp> slist<Tp>::~slist() { erase(begin(), end()); }

template <typename Tp> slist<Tp> &slist<Tp>::operator=(const slist &other) {
  if (&other == this)
    return *this;
  erase(begin(), end());
  for (const Tp &v : other)
    push_back(v);
  return *this;
}

template <typename Tp> slist<Tp> &slist<Tp>::operator=(slist &&other) {
  if (&other == this)
    return *this;
  if (alloc_ == other.alloc_) {
    erase(begin(), end());
    swap(other);
  } else
    operator=(other); // Copy assign
  return *this;
}

template <typename Tp> void slist<Tp>::swap(slist &other) noexcept {
  assert(alloc_ == other.alloc_);
  node_base *new_tail = other.empty() ? &head_ : other.ptail_;
  node_base *new_other_tail = empty() ? &other.head_ : ptail_;
  swap(head_.next_, other.head_.next_);
  swap(size_, other.size_);
  ptail_ = new_tail;
  other.ptail_ = new_other_tail;
}

template <typename Tp>
template <typename... Args>
typename slist<Tp>::iterator slist<Tp>::emplace(iterator i, Args &&... args) {
  node *new_node = static_cast<node *>(
      alloc_.resource()->allocate(sizeof(node), alignof(node)));
  try {
    alloc_.construct(addressof(new_node->value_), forward<Args>(args)...);
  } catch (...) {
    // Recover resources if exception on constructor call.
    alloc_.resource()->deallocate(new_node, sizeof(node), alignof(node));
    throw;
  }

  new_node->next_ = i.prev_->next_;
  i.prev_->next_ = new_node;
  if (i.prev_ == ptail_)
    ptail_ = new_node; // Added at end
  ++size_;
  return i;
}

template <typename Tp>
typename slist<Tp>::iterator slist<Tp>::erase(iterator b, iterator e) {
  node *erase_next = b.prev_->next_;
  node *erase_past = e.prev_->next_; // one past last erasure
  if (nullptr == erase_past)
    ptail_ = b.prev_;          // Erasing at tail
  b.prev_->next_ = erase_past; // splice out sublist
  while (erase_next != erase_past) {
    node *old_node = erase_next;
    erase_next = erase_next->next_;
    --size_;
    // alloc_.destroy(addressof(old_node->value_));
    addressof(old_node->value_)->~Tp();
    alloc_.resource()->deallocate(old_node, sizeof(node), alignof(node));
  }

  return b;
}

#endif // ! defined(INCLUDED_SLIST_DOT_H)
