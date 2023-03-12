//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Cost of CRTP approach
//
//----------------------------------------------------------------------------

template <typename Derived> struct Base {
  void foo() { static_cast<Derived *>(this)->foo(); }
};

struct Derived : Base<Derived> {
  void foo();
};

template <typename T> void call(Base<T> &b) { b.foo(); }

template void call(Base<Derived> &b);

template <typename T> void buz(Base<T> &);

void bar() {
  Derived d;
  buz(d);
}