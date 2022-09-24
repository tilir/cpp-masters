//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// parser.cppm -- bison related module
//
//------------------------------------------------------------------------------

module;

#include <cstring>

// inevitable hack
#include "yyret.h"

export module parser;

import<iostream>;
import<sstream>;
import<string>;

int CurrentInlinePos = 0;

export void PrintError(char const *Errmsg) {
  std::cerr << "Error: " << Errmsg << " - Line " << yylineno << ", Column "
            << CurrentInlinePos << std::endl;
  throw std::runtime_error("parse error");
}

export void BeginToken(char *t, int *yyinlinePos) {
  yylloc.first_line = yylineno;
  yylloc.first_column = *yyinlinePos;
  yylloc.last_line = yylineno;
  *yyinlinePos += strlen(t);
  yylloc.last_column = *yyinlinePos;
  CurrentInlinePos = *yyinlinePos;
}