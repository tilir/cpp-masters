//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Extended modules: shows transitive export
//
//-----------------------------------------------------------------------------

export module first_extended;

export import first_module;

namespace hello {

export int buz() { return bar(); }

} // namespace hello
