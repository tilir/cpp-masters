//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// hello.cc -- investigate additional cost of headers
//
// try:
// > clang++ -save-temps -S hello.cc
// > clang++ -save-temps -S hello.cc -stdlib=libc++
//
// also try:
// > clang++ -save-temps -H -S hello.cc -stdlib=libc++
//
//------------------------------------------------------------------------------

#include <iostream>

int main() { std::cout << "Hello" << std::endl; }