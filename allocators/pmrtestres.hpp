//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple PMR test resource
//
//----------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <memory_resource>
#include <vector>

class test_resource : public std::pmr::memory_resource {
  struct allocation_rec {
    void *ptr_;
    size_t nbytes_, nalign_;
  };

  std::pmr::memory_resource *parent_;
  std::pmr::vector<allocation_rec> blocks_;

public:
  explicit test_resource(
      std::pmr::memory_resource *parent = std::pmr::get_default_resource())
      : parent_(parent), blocks_(parent) {}

  std::pmr::memory_resource *parent() const { return parent_; }

  ~test_resource() {
    std::cout << "leaked " << blocks_.size() << " blocks" << std::endl;
    for (auto &&b : blocks_) {
      std::cout << "\t ptr:" << b.ptr_ << std::endl;
      parent_->deallocate(b.ptr_, b.nbytes_, b.nalign_);
    }
  }

protected:
  void *do_allocate(size_t bytes, size_t alignment) override {
    void *ret = parent_->allocate(bytes, alignment);
    std::cout << "a: " << bytes << "\t to: " << ret << std::endl;
    blocks_.push_back(allocation_rec{ret, bytes, alignment});
    return ret;
  }

  void do_deallocate(void *p, size_t bytes, size_t alignment) override {
    auto i = find_if(blocks_.begin(), blocks_.end(),
                     [p](const allocation_rec &r) { return r.ptr_ == p; });
    if (i == blocks_.end())
      throw std::invalid_argument("deallocate: Invalid pointer");
    blocks_.erase(i);
    parent_->deallocate(p, bytes, alignment);
  }

  bool do_is_equal(const memory_resource &other) const noexcept override {
    return this == &other;
  }
};
