CC = cl
CFLAGS = /nologo /W3 /DDLL_EXPORTS
LINKER = link
LINKFLAGS = /nologo /dll

build: so_stdio.dll so_stdio.lib

so_stdio.lib: so_stdio.obj
	lib /nologo /out:$@ $**

so_stdio.dll: so_stdio.obj
	$(LINKER) $(LINKFLAGS) /out:$@ /implib:so_stdio.lib $**

so_stdio.obj: so_stdio.c
	$(CC) $(CFLAGS) /c $**

.PHONY: clean

clean:
	del /F *.obj *.lib *.dll