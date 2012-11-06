CC=g++
CFLAGS=-Wall -pedantic -ansi -O3
LIBS=

all: bin/par_gen bin/par_view bin/par_solve

bin/par_gen: obj/par_gen.o obj/common.o obj/Instance.o 
	g++ $(CFLAGS) obj/par_gen.o obj/common.o obj/Instance.o -obin/par_gen

bin/par_view: obj/par_view.o obj/common.o obj/Instance.o 
	g++ $(CFLAGS) obj/par_view.o obj/common.o obj/Instance.o -obin/par_view

bin/par_solve: obj/par_solve.o obj/common.o obj/Instance.o obj/Solution.o
	g++ $(CFLAGS) obj/par_solve.o obj/common.o obj/Instance.o obj/Solution.o \
		-obin/par_solve

obj/par_gen.o: src/par_gen.cpp
	g++ -c $(CFLAGS) src/par_gen.cpp -oobj/par_gen.o

obj/par_view.o: src/par_view.cpp
	g++ -c $(CFLAGS) src/par_view.cpp -oobj/par_view.o

obj/par_solve.o: src/par_solve.cpp
	g++ -c $(CFLAGS) src/par_solve.cpp -oobj/par_solve.o
	
obj/common.o: src/common.cpp
	g++ -c $(CFLAGS) src/common.cpp -oobj/common.o

obj/Instance.o: src/Instance.cpp
	g++ -c $(CFLAGS) src/Instance.cpp -oobj/Instance.o

obj/Solution.o: src/Solution.cpp
	g++ -c $(CFLAGS) src/Solution.cpp -oobj/Solution.o
	
clean:
	rm bin/* obj/*
