// @Copyright 2021 Mihalcea Adrian - 334CA

#include <mpi.h>

using namespace std;

void Master(char* filename);
void Worker(int genre);

int main(int argc, char *argv[]) {
    int numtasks, rank;

    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        Master(argv[1]);
    } else {
        Worker(rank);
    }

    MPI_Finalize();
}
