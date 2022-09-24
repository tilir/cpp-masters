//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// yyret.h -- some almost inevitable hack
//
//------------------------------------------------------------------------------

// not required: it MUST be included twice
// #pragma once

import node;
import<string>;

#define YYERROR_VERBOSE 1

// function to run after parse
void wholeProgramAction();

struct yyRet {
  std::string name;
  int value;
  int linePos;
  int inLinePos;
  Node::INode *treeNode;
  Node::Operation op;
};

#define YYSTYPE yyRet
#include "compiler.cpp.h"

extern FILE *yyin;

int yylex();
int yyerror(char const *);

// line number diagnostics
extern int yylineno;