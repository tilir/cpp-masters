//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// two-phase instantiation: export import required here
//
//-----------------------------------------------------------------------------

export module twophase;

#ifdef CORR
export import<iostream>;
#else
import<iostream>;
#endif

template <typename T> void dump(T val) { std::cout << val << std::endl; }
export template <typename T> void output(T item) { dump(item); }