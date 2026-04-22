#include <stdio.h>
#include <mpi.h>

int initialise_mpi(int argc, char **argv, int *my_rank, int *uni_size);
void root_receive_task(int my_rank, int uni_size);
void sender_task(int my_rank, int uni_size);
void handle_small_universe(int uni_size);
void choose_task(int my_rank, int uni_size);

int main(int argc, char **argv)
{
    int my_rank = 0;
    int uni_size = 0;

    initialise_mpi(argc, argv, &my_rank, &uni_size);
    choose_task(my_rank, uni_size);
    MPI_Finalize();

    return 0;
}

int initialise_mpi(int argc, char **argv, int *my_rank, int *uni_size)
{
    int ierror = 0;

    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, uni_size);

    return ierror;
}

void root_receive_task(int my_rank, int uni_size)
{
    int recv_message = 0;
    int count = 1;
    int source = 0;
    int tag = 0;
    MPI_Status status;

    for (int their_rank = 1; their_rank < uni_size; their_rank++)
    {
        source = their_rank;
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

        printf("Hello, I am %d of %d. Received %d from Rank %d\n",
               my_rank, uni_size, recv_message, source);
    }
}

void sender_task(int my_rank, int uni_size)
{
    int send_message = 0;
    int count = 1;
    int dest = 0;
    int tag = 0;

    send_message = my_rank * 10;

    MPI_Ssend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

    printf("Hello, I am %d of %d. Sent %d to Rank %d\n",
           my_rank, uni_size, send_message, dest);
}

void handle_small_universe(int uni_size)
{
    printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
}

void choose_task(int my_rank, int uni_size)
{
    if (uni_size > 1)
    {
        if (my_rank == 0)
        {
            root_receive_task(my_rank, uni_size);
        }
        else
        {
            sender_task(my_rank, uni_size);
        }
    }
    else
    {
        handle_small_universe(uni_size);
    }
}
