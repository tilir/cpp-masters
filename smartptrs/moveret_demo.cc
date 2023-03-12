//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Good vs bad assembler for std move on return
//
//----------------------------------------------------------------------------

#include <utility>

struct Foo {
  Foo();
  Foo(const Foo &) = delete;
  Foo(Foo &&);
  Foo &append();
};

Foo appendFoo(Foo f) {
#ifdef IMPLICIT_MOVE
  f.append();
  return f;
#else
  return std::move(f.append());
#endif
}
