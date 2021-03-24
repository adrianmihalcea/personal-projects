CC = gcc
CFLAGS = -Wall -Wextra -std=c89 -g -ansi -pedantic -Iincludes/
EXEC = so-cpp

build: main.o hashmap.o ifs.o includes.o process.o
	$(CC) $(CFLAGS) $^ -o $(EXEC)

main.o: main.c

hashmap.o: hashmap.c

ifs.o: ifs.c

includes.o: includes.c

process.o: process.c

pack:
	zip Tema1SO_MihalceaAdrian_334CA.zip *.c Makefile GNUmakefile -r includes

.PHONY: clean

run:
	./so-cpp -I includes/ in.txt -o out.txt

clean:
	rm *.o so-cpp
