# Distributed and parallel text parser

## Intro
The code provided was written as an assignment for my Parallel and Distributed
Computing course. It uses a distributed system based on the MPI library to
apply a few transformations on a piece of text, described below.

## Compiling and running
Although it is C++ code, it needs to be compiled using mpic++ and run with
mpirun because it uses the MPI library. A makefile is also included.

The program expects one command-line argument, which specifies its input file.
Some sample inputs are provided.

## Features
The program receives a text file corresponding to 4 types of paragraphs
(paragraph type written as the paragraph heading), corresponding to the 4
worker nodes. The paragraphs will get transformed according to the simple rules
below.

Horror paragraphs get their consonnants doubled, comedy paragraphs get every
second letter capitalized, fantasy paragraphs get the first letter of each word
capitalized and science-fiction have every seventh word flipped.

## Technology
The program is written in C++, using data structures from the STL. It makes use
of the MPI library to create multiple nodes for our distributed system and to
communicate between, while std::thread is used to run each node on the available
number of threads.

## Implementation
The program is broken down into 5 nodes - the master node and 4 worker nodes for
each type of paragraph.

The master node starts 4 threads, for each paragraph type. These threads go
through the input file line by line, read the paragraphs of their type, send
them to the corresponding worker using MPI_Send() and receive the processed
paragraphs using MPI_Recv(). After the 4 threads finish their execution, the
main worker thread writes the output to the output file.

Every worker node creates its reader thread, which creates the other P-1
processing threads (P = available threads). The reader thread receives the
paragraphs from the corresponding master thread and assigns groups of 20 lines
to processor threads equitably.

The processor thread receives a reference to a vector of lines (a subset of the
paragraph) and applies the desired transformation on each line.

## Scalability (P = number of threads per node)
| Test | serial | P = 6 | P = 12 |
| :------------- | :----------: | :----------: | -----------: |
| Test 1 (1KB) | 0.009s | 0.365s | 0.365s |
| Test 2 (2,407KB) | 0.211s | 1.518s | 0.465s |
| Test 3 (7,489KB) | 0.600s | 0.615s | 0.616s |
| Test 4 (138,748KB) | 10.377s | 5.027s | 4.825s |
| Test 5 (199,991KB) | 14.761s | 7.032s | 6.880s |

Serial represents a single-node single-thread implementation of the same code.

For small text files, the overhead created by starting the nodes and threads
and communication between the nodes makes the distributed solution slower
than the linear implementation. However, for larger texts (> 10 MB), the
distributed solution that also makes use of multi-threading becomes
increasingly more efficient.
