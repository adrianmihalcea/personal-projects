CC = cl
CFLAGS = /Iincludes\ /MD /nologo
EXEC = so-cpp.exe
OBJ_LIST = main.obj hashmap.obj ifs.obj includes.obj process.obj

build: $(OBJ_LIST)
	$(CC) /Feso-cpp.exe $(OBJ_LIST)

main.obj: main.c
	$(CC) /c main.c $(CFLAGS)

hashmap.obj: hashmap.c
	$(CC) /c hashmap.c $(CFLAGS)

ifs.obj: ifs.c
	$(CC) /c ifs.c $(CFLAGS)

includes.obj: includes.c
	$(CC) /c includes.c $(CFLAGS)

process.obj: process.c
	$(CC) /c process.c $(CFLAGS)

.PHONY: clean

clean:
        del *.obj so-cpp.exe