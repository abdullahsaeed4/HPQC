#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int myRank, uniSize;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &uniSize);

    printf("Hello, I am process %d of %d\n", myRank, uniSize);

    MPI_Finalize();
    return 0;
}
