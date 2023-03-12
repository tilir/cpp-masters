//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Cost of virtual functions
//
//----------------------------------------------------------------------------

struct Base {
  virtual void foo() = 0;
  virtual ~Base() {}
};

struct Derived : Base {
  void foo() override;
};

void call(Base &b) { b.foo(); }

void buz(Base &);

void bar() {
  Derived d;
  buz(d);
}