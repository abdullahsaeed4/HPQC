#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);
void check_uni_size(int uni_size);
void root_task(int num_pings);
void client_task(int num_pings);

int main(int argc, char **argv)
{
    int my_rank = 0;
    int uni_size = 0;
    int num_pings = 0;

    num_pings = check_args(argc, argv);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

    check_uni_size(uni_size);

    if (my_rank == 0)
    {
        root_task(num_pings);
    }
    else
    {
        client_task(num_pings);
    }

    MPI_Finalize();
    return 0;
}

int check_args(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [NUM_PINGS]\n", argv[0]);
        exit(-1);
    }

    return atoi(argv[1]);
}

void check_uni_size(int uni_size)
{
    if (uni_size != 2)
    {
        fprintf(stderr, "Error: this program must be run with exactly 2 processes\n");
        exit(-1);
    }
}

void root_task(int num_pings)
{
    int counter = 0;
    int count = 1;
    int dest = 1;
    int source = 1;
    int tag = 0;
    MPI_Status status;

    double start_time = MPI_Wtime();

    while (counter < num_pings)
    {
        MPI_Send(&counter, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
        MPI_Recv(&counter, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    }

    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    double average_time = elapsed_time / num_pings;

    printf("Final counter: %d\n", counter);
    printf("Elapsed time: %f s\n", elapsed_time);
    printf("Average round-trip time: %f s\n", average_time);
}

void client_task(int num_pings)
{
    int counter = 0;
    int count = 1;
    int dest = 0;
    int source = 0;
    int tag = 0;
    MPI_Status status;

    while (counter < num_pings)
    {
        MPI_Recv(&counter, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        counter++;
        MPI_Send(&counter, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }
}
