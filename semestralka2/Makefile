CC=mpiCC
CFLAGS=-Wall -Wno-long-long -pedantic -ansi -O3 -ggdb
LIBS=

all: bin/par_gen bin/par_view bin/par_solve

bin/par_gen: obj/par_gen.o obj/common.o obj/Instance.o 
	$(CC) $(CFLAGS) obj/par_gen.o obj/common.o obj/Instance.o -obin/par_gen

bin/par_view: obj/par_view.o obj/common.o obj/Instance.o 
	$(CC) $(CFLAGS) obj/par_view.o obj/common.o obj/Instance.o -obin/par_view

bin/par_solve: obj/par_solve.o obj/common.o obj/Instance.o obj/Solution.o obj/Worker.o
	$(CC) $(CFLAGS) obj/par_solve.o obj/common.o obj/Instance.o obj/Solution.o \
		obj/Worker.o -obin/par_solve

obj/par_gen.o: src/par_gen.cpp
	$(CC) -c $(CFLAGS) src/par_gen.cpp -oobj/par_gen.o

obj/par_view.o: src/par_view.cpp
	$(CC) -c $(CFLAGS) src/par_view.cpp -oobj/par_view.o

obj/par_solve.o: src/par_solve.cpp
	$(CC) -c $(CFLAGS) src/par_solve.cpp -oobj/par_solve.o
	
obj/common.o: src/common.cpp
	$(CC) -c $(CFLAGS) src/common.cpp -oobj/common.o

obj/Instance.o: src/Instance.cpp
	$(CC) -c $(CFLAGS) src/Instance.cpp -oobj/Instance.o

obj/Solution.o: src/Solution.cpp
	$(CC) -c $(CFLAGS) src/Solution.cpp -oobj/Solution.o

obj/Worker.o: src/Worker.cpp
	$(CC) -c $(CFLAGS) src/Worker.cpp -oobj/Worker.o
	
clean:
	rm bin/* obj/*
