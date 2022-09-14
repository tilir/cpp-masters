#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------

result: maxuser1.o maxuser2.o max.o
	ar cr libmax.a maxuser1.o maxuser2.o max.o
	$(CXX) main.cc -L. -lmax -o $@
	objdump -d -M intel libmax.a > libmax.dis

max.o : max.cc max.hpp
	$(CXX) -O2 max.cc -c
	objdump -d -M intel max.o > max.dis

maxuser1.o : maxuser1.cc max.hpp
	$(CXX) maxuser1.cc -c
	objdump -d -M intel maxuser1.o > maxuser1.dis

maxuser2.o : maxuser2.cc max.hpp
	$(CXX) -O1 maxuser2.cc -c
	objdump -d -M intel maxuser2.o > maxuser2.dis

.PHONY: winclean
winclean:
	del *.o *.a a.exe result.exe *.dis

.PHONY: clean
clean:
	rm -rf *.o *.a a.out result *.dis
