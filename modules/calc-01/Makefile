#-----------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#-----------------------------------------------------------------------------
#
# Makefile: trivial build system
#
#-----------------------------------------------------------------------------

ADDFLAGS = -g -O0 -Wno-unused -Wno-unused-function -Wno-unused-label -Werror -Wall -Wextra

.PHONY: all
all: paracli

paracli: driver.cpp lexer.o parser.o node.o
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) -o $@ lexer.o parser.o node.o driver.cpp

parser.o: parser.hpp compiler.cpp.h compiler.cpp
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) compiler.cpp -c -o $@

lexer.o: parser.hpp compiler.cpp.h lex.yy.c
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) lex.yy.c -c -o $@

node.o: INode.hpp Node.hpp Symtab.hpp Node.cpp
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) Node.cpp -c -o $@

compiler.cpp compiler.cpp.h: compiler.y
	bison -o compiler.cpp --defines=compiler.cpp.h -v compiler.y

lex.yy.c: scanner.l
	flex -8 scanner.l

.PHONY: clean
clean:
	rm -f lex* compiler.c* compiler.out* paracli *.o
