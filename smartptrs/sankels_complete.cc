//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Complete Sankel case -- all through allocator
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <random>
#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace {

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

} // namespace

namespace {

class polymorphic_allocator_delete {
  using palloc_t = std::pmr::polymorphic_allocator<std::byte>;
  palloc_t alloc_;

public:
  polymorphic_allocator_delete(palloc_t alloc) : alloc_(std::move(alloc)) {}
  template <typename T> void operator()(T *ptr) {
    std::pmr::polymorphic_allocator<T> talloc(alloc_);
    talloc.destroy(ptr);
    talloc.deallocate(ptr, 1);
  }
};

class Bar {
  std::pmr::string data{"data really long data string to break SSO"};
};

class Foo {
  std::unique_ptr<Bar, polymorphic_allocator_delete> bar_;

public:
  Foo() : bar_(nullptr, {{std::pmr::get_default_resource()}}) {
    std::pmr::polymorphic_allocator<Bar> alloc{
        std::pmr::get_default_resource()};
    Bar *const bar = alloc.allocate(1);
    alloc.construct(bar);
    bar_.reset(bar);
  }
};

} // namespace

TEST(smartptrs, sankels_complete) {
  static test_resource newdefault{std::pmr::new_delete_resource()};
  std::pmr::set_default_resource(&newdefault);
  std::cout << "---" << std::endl;
  std::pmr::vector<Foo> foos;
  foos.emplace_back();
  foos.emplace_back();
  std::cout << "---" << std::endl;
}
