//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple reachable module: shows simple reachable mechanics
//
//-----------------------------------------------------------------------------

export module reachable;

#if 0

// invisible but reachable
struct S {  
  int what() const { return 42; }
  S() = default;
  S(S&&) = default;
};

export using U = S;

export U foo() { return {}; }

#else

// much simpler example from C++20 [module.reach] not working

struct S{};
export using Y = S;
   
#endif
