//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// global fragment instantiation issues: header part
//
//-----------------------------------------------------------------------------

#include <iostream>

template <typename T> void dump(T val) { std::cout << val << std::endl; }
