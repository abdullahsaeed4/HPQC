#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size, N;
    int *vector = NULL;
    int local_sum = 0;
    int total_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2)
    {
        if (rank == 0)
        {
            printf("Usage: %s [N]\n", argv[0]);
        }
        MPI_Finalize();
        return -1;
    }

    N = atoi(argv[1]);

    if (N % size != 0)
    {
        if (rank == 0)
        {
            printf("Error: N must be divisible by number of processes\n");
        }
        MPI_Finalize();
        return -1;
    }

    int chunk = N / size;

    vector = malloc(N * sizeof(int));
    if (vector == NULL)
    {
        printf("Memory allocation failed\n");
        MPI_Finalize();
        return -1;
    }

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            vector[i] = i * i;
        }
    }

    MPI_Bcast(vector, N, MPI_INT, 0, MPI_COMM_WORLD);

    int start = rank * chunk;
    int end = start + chunk;

    for (int i = start; i < end; i++)
    {
        local_sum += vector[i];
    }

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Total sum: %d\n", total_sum);
    }

    free(vector);
    MPI_Finalize();
    return 0;
}
