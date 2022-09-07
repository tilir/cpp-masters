//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Working with C strings
//  Find all bugs and problems here.
//
//----------------------------------------------------------------------------

#include <cstring>

int main() {
  const char *cinv = "Hello, world";
  const char cmut[] = "Hello, world";
  char *cheap = new char[14]{"Hello, world"};  
  cheap = cinv; 
  cinv = 0; 
  cmut = cheap;
  delete cheap;
}
