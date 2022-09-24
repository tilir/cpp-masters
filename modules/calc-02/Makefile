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

BASEFLAGS = -g -O0 -Wall -Wextra -Wno-unused -Wno-unused-function -Wno-unused-label -Wno-unneeded-internal-declaration -Werror
ADDFLAGS = $(BASEFLAGS) --std=c++20 -fmodules --stdlib=libc++
PREFLAGS = $(ADDFLAGS) --precompile

.PHONY: all
all: paracli

paracli: driver.cpp lexer.o parser.o node.o node.pcm
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) -fmodule-file=node.pcm -o $@ lexer.o parser.o node.o driver.cpp

parser.o: parser.hpp compiler.cpp.h compiler.cpp node.pcm
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) -fmodule-file=node.pcm compiler.cpp -c -o $@

lexer.o: parser.hpp compiler.cpp.h lex.yy.c node.pcm
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) -fmodule-file=node.pcm -x "c++" lex.yy.c -c -o $@

node.o: Node.cpp node.pcm symtab.pcm
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) -fmodule-file=node.pcm -fmodule-file=symtab.pcm Node.cpp -c -o $@

node.pcm: Node.cppm
	$(CXX) $(PREFLAGS) $(CXXFLAGS) Node.cppm -o node.pcm

symtab.pcm: Symtab.cppm node.pcm
	$(CXX) $(PREFLAGS) $(CXXFLAGS) Symtab.cppm -fmodule-file=node.pcm -o symtab.pcm

compiler.cpp compiler.cpp.h: compiler.y
	bison -o compiler.cpp --defines=compiler.cpp.h -v compiler.y

lex.yy.c: scanner.l
	flex -8 scanner.l

.PHONY: clean
clean:
	rm -f lex* compiler.c* compiler.out* paracli *.o *.pcm
