//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Sutter case for the exception safety
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {

struct Controllable {
  static int control;
  int *resource_;
  Controllable(int x = 42) : resource_(new int(x)) {}
  Controllable(Controllable &&rhs) noexcept : resource_(rhs.resource_) {
    rhs.resource_ = nullptr;
  }
  Controllable &operator=(Controllable &&rhs) noexcept {
    std::swap(resource_, rhs.resource_);
    return *this;
  }
  Controllable(const Controllable &rhs) : resource_(new int(*rhs.resource_)) {
    if (control == 0) {
      control = 5;
      std::cout << "Control reached\n";
      throw std::bad_alloc{};
    }
    control -= 1;
  }
  Controllable &operator=(const Controllable &rhs) {
    Controllable tmp(rhs);
    std::swap(*this, tmp);
    return *this;
  }

  ~Controllable() { delete resource_; }
};

int Controllable::control = 5;

template <typename T> void construct(T *p, const T &rhs) { new (p) T(rhs); }
template <typename T> void construct(T *p, T &&rhs) {
  new (p) T(std::move(rhs));
}

template <class T> void destroy(T *p) { p->~T(); }

template <typename FwdIter> void destroy(FwdIter first, FwdIter last) {
  while (first != last)
    destroy(&*first++);
}

template <typename T> struct MyVectorBuf {
protected:
  T *buffer_;
  size_t capacity_ = 0, size_ = 0;

protected:
  MyVectorBuf(const MyVectorBuf &) = delete;
  MyVectorBuf &operator=(const MyVectorBuf &) = delete;
  MyVectorBuf(MyVectorBuf &&rhs) noexcept
      : buffer_(rhs.buffer_), capacity_(rhs.capacity_), size_(rhs.size_) {
    rhs.buffer_ = nullptr;
    rhs.capacity_ = 0;
    rhs.size_ = 0;
  }

  MyVectorBuf &operator=(MyVectorBuf &&rhs) noexcept {
    std::swap(buffer_, rhs.buffer_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_, rhs.size_);
    return *this;
  }

  MyVectorBuf(size_t sz = 0)
      : buffer_((sz == 0) ? nullptr
                          : static_cast<T *>(::operator new(sizeof(T) * sz))),
        capacity_(sz) {}

  ~MyVectorBuf() {
    destroy(buffer_, buffer_ + size_);
    ::operator delete(buffer_);
  }
};

template <typename T> struct MyVector : private MyVectorBuf<T> {
  using MyVectorBuf<T>::size_;
  using MyVectorBuf<T>::capacity_;
  using MyVectorBuf<T>::buffer_;

  explicit MyVector(size_t sz = 0) : MyVectorBuf<T>(sz) {}

  MyVector(MyVector &&rhs) = default;
  MyVector &operator=(MyVector &&rhs) = default;

  MyVector(const MyVector &rhs) : MyVectorBuf<T>(rhs.size_) {
    while (size_ < rhs.size_) {
      construct(buffer_ + size_, rhs.buffer_[size_]);
      size_ += 1;
    }
  }

  MyVector &operator=(const MyVector &rhs) {
    MyVector tmp(rhs);
    std::swap(*this, tmp);
    return *this;
  }

  T top() const {
    if (size_ < 1)
      throw std::runtime_error("top failed: vector is empty");
    return buffer_[size_ - 1];
  }

  void pop() {
    if (size_ < 1)
      throw std::runtime_error("pop failed: vector is empty");
    size_ -= 1;
    destroy(buffer_ + size_);
  }

  void push(const T &t) {
    T t2(t);
    push(std::move(t2));
  }

  void push(T &&t) {
    assert(size_ <= capacity_);
    static_assert(std::is_nothrow_move_constructible<T>::value);
    static_assert(std::is_nothrow_move_assignable<T>::value);
    if (size_ == capacity_) {
      std::cout << "Realloc\n";
      MyVector tmp(capacity_ * 2 + 1);
      while (tmp.size_ < size_)
        tmp.push(std::move(buffer_[tmp.size_]));
      tmp.push(std::move(t));
      std::swap(*this, tmp);
    } else {
      construct(buffer_ + size_, std::move(t));
      size_ += 1;
    }
  }

  size_t size() const { return size_; }
  size_t capacity() const { return capacity_; }
};

} // namespace

TEST(threads, sutter_case) {
  try {
    Controllable c1, c2, c3;
    MyVector<Controllable> vv1(1);
    vv1.push(c1);
    vv1.push(c2);
    vv1.push(c3);
    std::cout << "Invoke copy ctor\n";
    MyVector<Controllable> vv2(vv1);
    std::cout << vv2.size() << std::endl;
  } catch (std::bad_alloc &) {
    std::cout << "Exception catched\n";
  }
}
