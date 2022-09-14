//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demo of instantiations, function bar
//
//----------------------------------------------------------------------------

#include "max.hpp"

int bar(int x, int y) { return max<int>(x + 1, y); }