//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Const on result not always great
//
//-----------------------------------------------------------------------------

struct Heavy {
  Heavy() {}
  Heavy(Heavy &&) {}
  Heavy(const Heavy &) = delete;
};

const Heavy foo() {
  Heavy x;
  return x;
}

Heavy bar() {
  Heavy x;
  return x;
}

void buz() {
  Heavy y = foo();
  Heavy z = bar();
}