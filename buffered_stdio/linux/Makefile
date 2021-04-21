CC = gcc
CFLAGS = -Wall -Wextra -g
LIB = libso_stdio.so

build: so_stdio.o
	$(CC) $(CFLAGS) -shared $^ -o $(LIB)

so_stdio.o: so_stdio.c
	$(CC) $(CFLAGS) -fPIC -c $^

pack:
	zip Tema2SO_MihalceaAdrian_334CA.zip so_stdio.c so_stdio.h Makefile README

.PHONY: clean

clean:
	rm *.o $(LIB)
