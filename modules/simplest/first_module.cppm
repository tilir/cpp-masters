//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// First simple module: shows simple export and internal details
//
//-----------------------------------------------------------------------------

export module first_module;

namespace hello {

int foo(int x) { return x; }

export int e = 42;

export int bar() { return foo(e); }

} // namespace hello