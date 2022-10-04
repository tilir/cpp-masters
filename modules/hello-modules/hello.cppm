//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Hello world module: import from standard library
//
//-----------------------------------------------------------------------------

export module helloworld;

import<iostream>;

export void hello() { std::cout << "Hello world!\n"; }
