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

# note parser.pcm added explicitly to comiplation line, isn't it weird?
paracli: driver.cpp lexer.o parser.o node.o parser.pcm
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) -fmodule-file=parser.pcm -o $@ lexer.o parser.o node.o driver.cpp parser.pcm

parser.o: compiler.cpp.h compiler.cpp parser.pcm
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) -fmodule-file=parser.pcm compiler.cpp -c -o $@

lexer.o: compiler.cpp.h lex.yy.c parser.pcm
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) -fmodule-file=parser.pcm -x "c++" lex.yy.c -c -o $@

node.o: Node.cpp node.pcm symtab.pcm
	$(CXX) $(ADDFLAGS) $(CXXFLAGS) -fmodule-file=node.pcm -fmodule-file=symtab.pcm Node.cpp -c -o $@

parser.pcm: parser.cppm node.pcm compiler.cpp.h yyret.h
	$(CXX) $(PREFLAGS) $(CXXFLAGS) parser.cppm -fmodule-file=node.pcm -o $@

symtab.pcm: Symtab.cppm node.pcm
	$(CXX) $(PREFLAGS) $(CXXFLAGS) Symtab.cppm -fmodule-file=node.pcm -o $@

node.pcm: Node.cppm
	$(CXX) $(PREFLAGS) $(CXXFLAGS) Node.cppm -o node.pcm

compiler.cpp compiler.cpp.h: compiler.y
	bison -o compiler.cpp --defines=compiler.cpp.h -v compiler.y

lex.yy.c: scanner.l
	flex -8 scanner.l

.PHONY: clean
clean:
	rm -f lex* compiler.c* compiler.out* paracli *.o *.pcm
