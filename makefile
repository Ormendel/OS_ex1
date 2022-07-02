CC=gcc
AR=ar
FLAGS= -Wall -g

all: run server libmain.so

server: libmain.a
	$(CC) $(FLAGS) -lm -o server server.o -L. -lmain

run: libmain.a
	$(CC) $(FLAGS) -lm -o run main.o -L. -lmain

main.o: main.c main.h
	$(CC) $(FLAGS) -O -c main.c

server.o: server.c
	$(CC) $(FLAGS) -O -c server.c

libmain.a: main.o server.o
	$(AR) rcs libmain.a main.o

libmain.so: main.c main.h
	$(CC) $(FLAGS) main.c -o libmain.so -fPIC -shared

libs: libmain.a libmain.so

.PHONY: clean all libs

clean:
	rm -f run server *.o *.a *.so
