PRG=gnu.exe
OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

GCC=g++
GCCFLAGS=-O3 -std=c++11 -pedantic 
DEFINE=

VALGRIND_OPTIONS=-q --leak-check=full --show-reachable=yes
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

OBJECTS0=pl.cpp
DRIVER0=driver.cpp

gcc0:
	g++ $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $(PRG)
debugVer:
	g++ $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -DDEBUG -o $(PRG)
0 1 2 3 4 5 6 7 8 9 10 11 12 13:
	@echo "should run in less than 1000 ms"
	./$(PRG) $@ >studentout$@
	@echo "lines after the next are mismatches with master output -- see out$@"
	diff out$@ studentout$@ $(DIFF_OPTIONS)
  
mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8:
	@echo "should run in less than 3000 ms"
	valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2>difference$@
	@echo "lines after this are memory errors"; cat difference$@
  
clean:
	rm -f *.exe *.o *.obj
