// @Copyright 2021 Mihalcea Adrian - 334CA

#include <mpi.h>

#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <sstream>

#include "genre_functions.h"
#include "helpers.h"

using namespace std;

// calls the corresponding processing function to update the paragraph
void Worker_Process(vector<string>& lines, int id, int nr_processors, int genre) {
	int i = id * kLinesPerThread;
	while (i < lines.size()) {
		for (int j = 0; j < kLinesPerThread && i + j < lines.size(); ++j) {
			switch (genre) {
			case HORROR:
				Change_Horror(lines[i + j]);
				break;
			case COMEDY:
				Change_Comedy(lines[i + j]);
				break;
			case SF:
				Change_SF(lines[i + j]);
				break;
			case FANTASY:
				Change_Fantasy(lines[i + j]);
				break;
			default:
				lines[i+j] = "procesat de " + to_string(id);
			}
		}
		i += nr_processors * kLinesPerThread;
	}
}

// sends all the paragraphs back to master
void Send_Back(map<int, string>& ret) {
	for (auto const& x : ret) {
		int send = x.first;
		MPI_Send(&send, 1, MPI_INT, kMaster, 0, MPI_COMM_WORLD);
		int count = x.second.length() + 1;
		MPI_Send(&count, 1, MPI_INT, kMaster, 0, MPI_COMM_WORLD);
		MPI_Send(x.second.c_str(), count, MPI_CHAR, kMaster, 0, MPI_COMM_WORLD);
	}
	// -1 to signal the end
	int send = -1;
	MPI_Send(&send, 1, MPI_INT, kMaster, 0, MPI_COMM_WORLD);
}

// Receives paragraphs from the corresponding master thread
void Worker_Read(int genre) {
	map<int, string> ret;
	vector<string> lines;
	int recv, par;
	// number of threads
	int P = thread::hardware_concurrency();
	while (true) {
		// for every paragraph
		MPI_Recv(&recv, 1, MPI_INT, kMaster, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		if (recv == -1) {
			// this worker is done, sends back its result
			Send_Back(ret);
			return;
		}

		int size;
		MPI_Recv(&size, 1, MPI_INT, kMaster, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		char* line = new char[size];
		MPI_Recv(line, size, MPI_CHAR, kMaster, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// parses the paragraph line by line (more efficient as there are fewer sends)
		istringstream paragraf(line);
		string temp;
		while (getline(paragraf, temp)) {
			lines.push_back(temp);
		}

		// processing
		{
			thread* threads = new thread[P - 1];
			for (int i = 0; i < P - 1 && i * kLinesPerThread < lines.size(); ++i) {
				threads[i] = thread(Worker_Process, ref(lines), i, P - 1, genre);
			}
			for (int i = 0; i < P - 1 && i * kLinesPerThread < lines.size(); ++i) {
				threads[i].join();
			}
		}

		temp = "";
		for (string& s : lines) {
			temp += s + " \n";
		}
		ret[recv] = temp;
		lines.clear();

		delete[] line;
	}
}

// create the worker thread, which will create the other threads
void Worker(int genre) {
	thread t(Worker_Read, genre);
	t.join();
}