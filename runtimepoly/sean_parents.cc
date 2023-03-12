//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Example based on Sean Parent's "Runtime polymorphism" talk.
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template <typename T>
void draw(const T &x, std::ostream &out, size_t position) {
  out << std::string(position, ' ') << x << std::endl;
}

class myclass_t {};

void draw(const myclass_t &, std::ostream &out, size_t position) {
  out << std::string(position, ' ') << "myclass_t" << std::endl;
}

class object_t;
using document_t = std::vector<object_t>;

void draw(const document_t &x, std::ostream &out, size_t position);

class object_t {
  struct concept_t {
    virtual ~concept_t() = default;
    virtual std::unique_ptr<concept_t> copy_() const = 0;
    virtual void draw_(std::ostream &, size_t) const = 0;
  };

  template <typename T> struct model final : concept_t {
    T data_;
    model(T x) : data_(std::move(x)) {}
    std::unique_ptr<concept_t> copy_() const override {
      return std::make_unique<model>(*this);
    }
    void draw_(std::ostream &out, size_t position) const override {
      ::draw(data_, out, position);
    }
  };

  std::unique_ptr<concept_t> self_;

public:
  template <typename T>
  object_t(T x) : self_(std::make_unique<model<T>>(std::move(x))) {}

  // copy ctor, move ctor and assignment
public:
  object_t(const object_t &x) : self_(x.self_->copy_()) {}
  object_t(object_t &&x) noexcept = default;
  object_t &operator=(object_t x) noexcept {
    self_ = std::move(x.self_);
    return *this;
  }

public:
  friend void draw(const object_t &x, std::ostream &out, size_t position) {
    x.self_->draw_(out, position);
  }
};

void draw(const document_t &x, std::ostream &out, size_t position) {
  out << std::string(position, ' ') << "<document>" << std::endl;
  for (const auto &e : x)
    draw(e, out, position + 2);
  out << std::string(position, ' ') << "</document>" << std::endl;
}

int main() {
  document_t document;
  document.emplace_back(0);
  document.emplace_back(std::string("Hello"));
  document.emplace_back(document);
  document.emplace_back(myclass_t{});
  draw(document, std::cout, 0);
}
