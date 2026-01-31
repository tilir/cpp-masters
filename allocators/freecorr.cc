//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Freelist allocator fixed
//
//----------------------------------------------------------------------------

#include <cstdio>
#include <limits>
#include <list>
#include <memory>
#include <vector>

#include "gtest/gtest.h"

// freelist_alloc.hpp (C++11)
// A minimal freelist allocator intended for node-based containers (e.g. std::list).
// Freelist is only used for allocate/deallocate with n == 1; other n go straight to heap.
// Not thread-safe. Over-aligned types (alignof(T) > alignof(max_align_t)) are rejected.

#include <cassert>
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <list>
#include <memory>
#include <new>
#include <vector>
#include <type_traits>
#include <utility>
#include <unordered_map>

#include "gtest/gtest.h"

namespace freelist_detail {

struct freelist_resource {
  // heads[size] points to a singly-linked list of freed blocks of exactly 'size' bytes.
  // The "next" pointer is stored in the first sizeof(void*) bytes of the freed block.
  std::unordered_map<std::size_t, void*> heads;

  // Optional counters (useful for your lecture demo / tests)
  std::size_t heap_allocs     = 0;
  std::size_t freelist_allocs = 0;
  std::size_t freelist_frees  = 0;

  ~freelist_resource() {
    // Release everything currently cached in the freelists.
    for (auto& kv : heads) {
      void* p = kv.second;
      while (p) {
        void* next = *reinterpret_cast<void**>(p);
        ::operator delete(p);
        p = next;
      }
      kv.second = nullptr;
    }
  }

  void* pop(std::size_t bytes) noexcept {
    auto it = heads.find(bytes);
    if (it == heads.end() || it->second == nullptr) {
      return nullptr;
    }
    void* p = it->second;
    it->second = *reinterpret_cast<void**>(p);
    return p;
  }

  void push(std::size_t bytes, void* p) noexcept {
    void*& head = heads[bytes];              // creates entry if absent
    *reinterpret_cast<void**>(p) = head;     // store next
    head = p;                                // new head
  }
};

} // namespace freelist_detail

template <class T>
class freelist_alloc {
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  template <class U>
  struct rebind { using other = freelist_alloc<U>; };

  // allocator_traits hooks
  using propagate_on_container_copy_assignment = std::false_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap            = std::true_type;
  using is_always_equal                        = std::false_type;

  // Owning ctor: creates a fresh freelist_resource.
  freelist_alloc()
      : owned_(new freelist_detail::freelist_resource),
        res_(owned_.get()) {
    static_assert(alignof(T) <= alignof(std::max_align_t),
                  "freelist_alloc: over-aligned types are not supported in C++11");
  }

  // Non-owning copy ctor: shares the resource via raw pointer (lifetime tied to the container).
  freelist_alloc(const freelist_alloc& other) noexcept
      : owned_(nullptr), res_(other.res_) {}

  // Cross-type converting ctor for rebind: MUST share the same resource.
  template <class U>
  freelist_alloc(const freelist_alloc<U>& other) noexcept
      : owned_(nullptr), res_(other.resource_ptr()) {
    static_assert(alignof(T) <= alignof(std::max_align_t),
                  "freelist_alloc: over-aligned types are not supported in C++11");
  }

  freelist_alloc(freelist_alloc&& other) noexcept
      : owned_(std::move(other.owned_)), res_(nullptr) {
    res_ = owned_ ? owned_.get() : other.res_;
    other.res_ = nullptr;
  }

  freelist_alloc& operator=(const freelist_alloc& other) noexcept {
    if (this == &other) return *this;
    // Copy-assignment does NOT propagate ownership; become non-owning alias.
    owned_.reset();
    res_ = other.res_;
    return *this;
  }

  freelist_alloc& operator=(freelist_alloc&& other) noexcept {
    if (this == &other) return *this;
    owned_ = std::move(other.owned_);
    res_ = owned_ ? owned_.get() : other.res_;
    other.res_ = nullptr;
    return *this;
  }

  ~freelist_alloc() = default;

  freelist_alloc select_on_container_copy_construction() const {
    // Copy-constructed container gets a fresh freelist (part of the container).
    return freelist_alloc{};
  }

  pointer allocate(size_type n) {
    if (n == 0) return nullptr;

    if (n == 1) {
      if (res_) {
        void* p = res_->pop(sizeof(T));
        if (p) {
          ++res_->freelist_allocs;
          printf("allocate from freelist\n");
          return static_cast<pointer>(p);
        }
      }
      // fallthrough to heap
      if (res_) ++res_->heap_allocs;
      printf("allocate from heap\n");
      return static_cast<pointer>(::operator new(sizeof(T)));
    }

    // Not using freelist for arrays.
    return static_cast<pointer>(::operator new(n * sizeof(T)));
  }

  void deallocate(pointer p, size_type n) noexcept {
    if (!p) return;

    if (n == 1 && res_) {
      ++res_->freelist_frees;
      printf("deallocate to freelist\n");
      res_->push(sizeof(T), p);
      return;
    }

    ::operator delete(p);
  }

  // Equality: same resource => interchangeable for deallocation and node moves.
  friend bool operator==(const freelist_alloc& a, const freelist_alloc& b) noexcept {
    return a.res_ == b.res_;
  }
  friend bool operator!=(const freelist_alloc& a, const freelist_alloc& b) noexcept {
    return !(a == b);
  }

  // Expose resource pointer for converting ctor
  freelist_detail::freelist_resource* resource_ptr() const noexcept { return res_; }

  // Optional: expose counters (for demos/tests)
  std::size_t heap_allocs() const noexcept     { return res_ ? res_->heap_allocs     : 0; }
  std::size_t freelist_allocs() const noexcept { return res_ ? res_->freelist_allocs : 0; }
  std::size_t freelist_frees() const noexcept  { return res_ ? res_->freelist_frees  : 0; }

private:
  template <class U> friend class freelist_alloc;

  // Owning instance keeps unique ownership; copies are non-owning aliases.
  std::unique_ptr<freelist_detail::freelist_resource> owned_;
  freelist_detail::freelist_resource* res_;
};

TEST(allocs, freelist) {
  std::list<int, freelist_alloc<int>> l;
  ASSERT_EQ(l.get_allocator().heap_allocs(), 0);
  ASSERT_EQ(l.get_allocator().freelist_allocs(), 0);
  ASSERT_EQ(l.get_allocator().freelist_frees(), 0);
  l.insert(l.begin(), 1); // from heap
  l.insert(l.begin(), 2); // from heap
  l.insert(l.begin(), 3); // from heap
  ASSERT_EQ(l.get_allocator().heap_allocs(), 3);
  ASSERT_EQ(l.get_allocator().freelist_allocs(), 0);
  ASSERT_EQ(l.get_allocator().freelist_frees(), 0);  
  l.remove(1); // remove to free list
  l.remove(2); // remove to free list
  l.remove(3); // remove to free list
  ASSERT_EQ(l.get_allocator().heap_allocs(), 3);
  ASSERT_EQ(l.get_allocator().freelist_allocs(), 0);
  ASSERT_EQ(l.get_allocator().freelist_frees(), 3);  
  l.insert(l.begin(), -1); // from free list
  l.insert(l.begin(), -2); // from free list
  l.insert(l.begin(), -3); // from free list
  ASSERT_EQ(l.get_allocator().heap_allocs(), 3);
  ASSERT_EQ(l.get_allocator().freelist_allocs(), 3);
  ASSERT_EQ(l.get_allocator().freelist_frees(), 3);  
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
