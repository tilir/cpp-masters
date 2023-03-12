//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Cost of virtual inheritance: triple indirection
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory_resource>

struct Base {
  int a = 1;
  virtual void foo() = 0;
  virtual ~Base() {}
};

struct Derived : virtual public Base {
  int b = 2;
  void foo() override;
};

int call_virtual(Derived &d) { return d.a; }

void buz(Base &);

void bar() {
  Derived d;
  buz(d);
}