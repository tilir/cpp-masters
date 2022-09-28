//-----------------------------------------------------------------------------
//
// Source code for MIPT course on informatics
// Page with slides: http://cs.mipt.ru/wp/?page_id=7775
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// ODR usage example (with slight clang/gcc disagreement)
//
//-----------------------------------------------------------------------------

struct S {
  static const int n = 5; // declaration
};

int x = S::n + 1; // n not ODR-used

int foo(const int *x) { return *x; }

#if 0
const int S::n; // definition
#endif

int y = foo(&S::n) + 1; // n ODR-used [class.static.data]

int main() {
  return y;
}