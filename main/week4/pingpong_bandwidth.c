#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_num_pings(int argc, char **argv);
int check_num_elements(int argc, char **argv);
void check_uni_size(int uni_size);
void initialise_array(int *array, int num_elements);
void root_task(int num_pings, int num_elements);
void client_task(int num_pings, int num_elements);

int main(int argc, char **argv)
{
    int my_rank = 0;
    int uni_size = 0;
    int num_pings = 0;
    int num_elements = 0;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s [NUM_PINGS] [NUM_ELEMENTS]\n", argv[0]);
        return -1;
    }

    num_pings = atoi(argv[1]);
    num_elements = atoi(argv[2]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

    check_uni_size(uni_size);

    if (my_rank == 0)
    {
        root_task(num_pings, num_elements);
    }
    else
    {
        client_task(num_pings, num_elements);
    }

    MPI_Finalize();
    return 0;
}

void check_uni_size(int uni_size)
{
    if (uni_size != 2)
    {
        fprintf(stderr, "Error: this program must be run with exactly 2 processes\n");
        exit(-1);
    }
}

void initialise_array(int *array, int num_elements)
{
    for (int i = 0; i < num_elements; i++)
    {
        array[i] = i;
    }
}

void root_task(int num_pings, int num_elements)
{
    int *message = malloc(num_elements * sizeof(int));
    if (message == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(-1);
    }

    initialise_array(message, num_elements);

    int dest = 1;
    int source = 1;
    int tag = 0;
    MPI_Status status;

    double start_time = MPI_Wtime();

    for (int counter = 0; counter < num_pings; counter++)
    {
        MPI_Send(message, num_elements, MPI_INT, dest, tag, MPI_COMM_WORLD);
        MPI_Recv(message, num_elements, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    }

    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    double average_time = elapsed_time / num_pings;

    printf("Message elements: %d\n", num_elements);
    printf("Message bytes: %ld\n", (long)(num_elements * sizeof(int)));
    printf("Elapsed time: %f s\n", elapsed_time);
    printf("Average round-trip time: %f s\n", average_time);

    free(message);
}

void client_task(int num_pings, int num_elements)
{
    int *message = malloc(num_elements * sizeof(int));
    if (message == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(-1);
    }

    int dest = 0;
    int source = 0;
    int tag = 0;
    MPI_Status status;

    for (int counter = 0; counter < num_pings; counter++)
    {
        MPI_Recv(message, num_elements, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        MPI_Send(message, num_elements, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }

    free(message);
}
