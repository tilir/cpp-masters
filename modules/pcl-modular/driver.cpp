//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// driver.cpp -- main entry point
//
//------------------------------------------------------------------------------

import<cstdlib>;
import<iostream>;
import<memory>;
import<string>;

// again inevitable for yyin?
#include "yyret.h"

import node;
import parser;

FILE *OpenFile(const char *Name) {
  FILE *F = fopen(Name, "r");
  if (!F) {
    perror("Cannot open file");
    throw std::runtime_error("No input file exists");
  }
  return F;
}

int main(int argc, char **argv) try {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " file.pcl" << std::endl;
    return 0;
  }
  auto fdeleter = [](FILE *f) { fclose(f); };
  std::unique_ptr<FILE, decltype(fdeleter)> F(OpenFile(argv[1]), fdeleter);
  yyin = F.get();
  std::unique_ptr<Node::IScope> CurrentScopeOwner{createScope()};
  CurrentScope = CurrentScopeOwner.get();
  yyparse();
} catch (const std::exception &e) {
  std::cerr << "Exception: " << e.what() << std::endl;
} catch (...) {
  std::cerr << "Exception unknown\n";
}