# Minimal C Preprocessor

## Intro
This is a personal implementation of a basic C preprocessor (subset of
directives implemented detailed below). It was written as homework for my
Operating Systems Course in 2021.

## Compiling and running
The code provided includes makefiles for both Linux (make, gcc) and Windows
(nmake, cl).

The program can be run as:
`so-cpp [-D <SYMBOL>[=<MAPPING>]] [-I <DIR>] [<INFILE>] [ [-o] <OUTFILE>]`

Thus, it supports defines and includes paths from the command line, along
with passing input and output files. If no input/ output files are found,
stdin/ stdout will be used for input and output.

## Supported directives
- #define <SYMBOL> [<MAPPING>]
	- stores a mapping and replaces all future encounters of the symbol with
	the mapping unless within a literal,
	- works within other directives;
- #if <CONDITION> / #elif <CONDITION> / #else / #endif
	- check if each branch has a condition that evaluates to a non-null
	numeral,
	- only supports literals and defines that are replaced with numerals by
	define directives;
- #ifdef <SYMBOL> / #ifndef <SYMBOL> / #else / #endif
	- same as above, but uses as truth condition whether the symbol has already
	been defined;
- #include "HEADER"
	- preprocesses the header file found either in the current directive or in
	one of the included paths and adds the preprocessed code at the current
	line in the original source
	- works recursively, supports include guards

## Implementation
My code relies on a basic implementation of a hashmap written by me in
hashmap.c for mapping defines. 100 buckets of variable size with a simple
hash function for strings ensures O(1) access for any reasonable number
of defines.

Ifs.c evaluates if and ifdef conditions and keeps the code from the correct
branch (evaluated as true). The lines kept will in turn be preprocessed.

Includes.c defines the data structure (basically a resizable array) for
storing include_paths. It also deals with adding include_paths and opening
an included file by first trying the current directory and then every included
path.

Process.c holds the main logic of the program, loading the file to be processed
into memory, tokenizing it using strcspn and then parsing every directive
found. It calls itself recursively for every included file.

Main.c allocates and deallocates memory, reads CLI arguments and calls the
process() function.

## Technology
Written entirely in C. As a requirement for my homework, and in order to ensure
cross-platform compatibility with the Visual Studio cl compiler, it was
written and compiled complying with the ANSI C89 standard.

All memory that is not used locally in a function is allocated dynamically, and
the whole program is programmed with a defensive mindset, checking for poor
arguments, memory failures, I/O errors and returning an appropriate error code.
