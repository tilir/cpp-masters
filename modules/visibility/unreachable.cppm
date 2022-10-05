//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple unreachable module: shows simple reachable mechanics
//
//-----------------------------------------------------------------------------

export module unreachable;

// invisible and unreachable
struct T; 

export T bar(int x);

module : private;

struct T {
  int x;
  int what() const { return x; }
};

T bar(int x) { return {x}; }
