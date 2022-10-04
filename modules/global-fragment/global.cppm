//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// global fragment instantiation issues
//
//-----------------------------------------------------------------------------

module;

#include "global.hpp"

export module global;

export template <typename T> void output(T item) { dump(item); }
