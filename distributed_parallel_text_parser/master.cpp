// @Copyright 2021 Mihalcea Adrian - 334CA

#include <mpi.h>

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "helpers.h"

using namespace std;

// Sends the complete paragraph to the correct worker
void Send_To(string& paragraph, int genre, int id) {
    MPI_Send(&id, 1, MPI_INT, genre, 0, MPI_COMM_WORLD);
    int size = paragraph.length() + 1;
    MPI_Send(&size, 1, MPI_INT, genre, 0, MPI_COMM_WORLD);
    MPI_Send(paragraph.c_str(), size, MPI_CHAR, genre, 0, MPI_COMM_WORLD);
    paragraph = "";
}

// Thread function for master, 1 thread for each genre
// Reads input and communicates with worker
void Master_Read(char* filename, int genre, map<int, string>& processed, int& max_id) {
    ifstream fin(filename);
    int par_id = -1, par_genre;
    string par;
    while(!fin.eof()) {
        // while there is more input, read first line of paragraph
        // to get genre
        string line;
        getline(fin, line);
        par_genre = GetGenreInt(line[0]);

        while (getline(fin, line)) {
            // every line gets added to the current paragraph
            if (!line.length()) {
                // end of paragraph
                ++par_id;
                if (par_genre == genre) {
                    Send_To(par, genre, par_id);
                }
                break;
            }

            if (par_genre == genre) {
                // only add paragraphs of the correct genre
                par += line + '\n';
            }
        }
    }

    // final paragraph doesn't end with an empty line
    ++par_id;
    if (par_genre == genre) {
        Send_To(par, genre, par_id);
    }
    if (genre == HORROR) {
        max_id = par_id;
    }

    // file done, stop corresponding worker
    int send = -1;
    MPI_Send(&send, 1, MPI_INT, genre, 0, MPI_COMM_WORLD);

    while (true) {
        int par_id;
        MPI_Recv(&par_id, 1, MPI_INT, genre, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (par_id == -1) {
            break;
        }

        if (par_id > max_id) {
            max_id = par_id;
        }

        int count;
        MPI_Recv(&count, 1, MPI_INT, genre, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        char* recv = new char[count];
        MPI_Recv(recv, count, MPI_CHAR, genre, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        processed[par_id] = kHeader[genre] + string(recv);
        delete[] recv;
    }
}

void Master(char* filename) {
    map<int, string> processed;
    int max_id = 0;

    // Read from files, send to workers and receive back
    {
        thread* threads = new thread[kNoGenres];
        for (int i = 0; i < kNoGenres; ++i) {
            threads[i] = thread(Master_Read, filename, i+1, ref(processed), ref(max_id));
        }

        for (int i = 0; i < kNoGenres; ++i) {
            threads[i].join();
        }
        delete[] threads;
    }

    // Write output to file
    {
        string out = filename;
        int dot = out.find('.', 1);
        out = out.substr(0, dot) + ".out";
        ofstream fout(out);

        for (int i = 0; i <= max_id; ++i) {
            fout << processed[i];
            if (i != max_id) {
                fout << '\n';
            }
        }

        fout.close();
    }
}
