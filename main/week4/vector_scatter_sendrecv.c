#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size, N;
    int *vector = NULL;
    int *local_vec = NULL;
    int local_sum = 0;
    int total_sum = 0;
    MPI_Status status;

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
    local_vec = malloc(chunk * sizeof(int));

    if (rank == 0)
    {
        vector = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++)
        {
            vector[i] = i * i;
        }
    }

    MPI_Scatter(vector, chunk, MPI_INT, local_vec, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < chunk; i++)
    {
        local_sum += local_vec[i];
    }

    if (rank == 0)
    {
        total_sum = local_sum;
        int recv_sum = 0;

        for (int p = 1; p < size; p++)
        {
            MPI_Recv(&recv_sum, 1, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
            total_sum += recv_sum;
        }

        printf("Total sum: %d\n", total_sum);
    }
    else
    {
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    free(local_vec);
    if (rank == 0)
    {
        free(vector);
    }

    MPI_Finalize();
    return 0;
}
