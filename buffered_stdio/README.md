# Buffered stdio implementation

## Intro
The code provided was written as an assignment for my Operating Systems course. It provides a simplified implementation of the stdio library.

## Compiling and running
The code provided in the two source file implements the functions declared in the so_stdio.h header file.

I have provided makefiles for both Linux (make, gcc) and Windows (nmake, cl), which generate a dynamic library (libso_stdio.so / so_stdio.dll).

## Supported functions
The library interfaces with the open file or process through the SO_FILE structure, mimicking the FILE implementation.

The SO_FILE stream relies on an internal read/write buffer to limit IO system calls, flushing to disk only when required.
- so_fopen: opens the file in the specified mode and allocates memory;
- so_fclose: closes the file descriptor and frees memory;
- so_fileno: returns file descriptor;
- so_fflush: flushes and clears the write buffer;
- so_fseek & so_ftell: moves the file cursor and clears the buffer & returns current buffer position;
- so_fgetc & so_fread: returns the oldest character in the buffer or refreshes the buffer if empty & calls so_fgets the specified number of bytes;
- so_fputc & so_fwrite: adds character to write buffer and flushes if necessary & calls so_fputc the specified number of bytes;
- so_feof & so_ferror: returns 0/1 if EOF or an error has been encountered so far;
- so_popen: forks to run the provided command as a new process, piped into the SO_FILE stream (either input / output);
- so_fclose: waits for the child process to exit and frees memory and descriptors.

## Technology
The library is implemented using platform-specific IO system calls: <br>
- Linux: open, read, write, lseek, pipe, dup, fork, exec
- Windows: CreateFile, ReadFile, WriteFile
