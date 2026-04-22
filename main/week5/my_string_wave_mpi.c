#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

typedef struct {
    int points;
    int cycles;
    int samples;
    char output_path[256];
} Args;

Args check_args(int argc, char **argv, int rank);
void print_header(FILE *out_file, int points);
void initialise_positions(double positions[], int n);
void print_positions(FILE *out_file, int step, double time, double positions[], int points);

int main(int argc, char **argv)
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Args args = check_args(argc, argv, rank);

    MPI_Bcast(&args.points, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&args.cycles, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&args.samples, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(args.output_path, 256, MPI_CHAR, 0, MPI_COMM_WORLD);

    int points = args.points;
    int cycles = args.cycles;
    int samples = args.samples;

    if (points % size != 0)
    {
        if (rank == 0)
        {
            fprintf(stderr, "Error: points must be divisible by number of processes\n");
        }
        MPI_Finalize();
        return -1;
    }

    int local_n = points / size;
    double *local_positions = malloc(local_n * sizeof(double));
    double *global_positions = NULL;

    if (local_positions == NULL)
    {
        fprintf(stderr, "Memory allocation failed on rank %d\n", rank);
        MPI_Finalize();
        return -1;
    }

    if (rank == 0)
    {
        global_positions = malloc(points * sizeof(double));
        if (global_positions == NULL)
        {
            fprintf(stderr, "Global allocation failed\n");
            free(local_positions);
            MPI_Finalize();
            return -1;
        }
        initialise_positions(global_positions, points);
    }

    MPI_Scatter(global_positions, local_n, MPI_DOUBLE,
                local_positions, local_n, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    FILE *out_file = NULL;
    if (rank == 0)
    {
        out_file = fopen(args.output_path, "w");
        if (out_file == NULL)
        {
            fprintf(stderr, "Could not open output file %s\n", args.output_path);
            free(local_positions);
            free(global_positions);
            MPI_Finalize();
            return -1;
        }
        print_header(out_file, points);
    }

    int total_steps = cycles * samples;

    for (int step = 0; step <= total_steps; step++)
    {
        double time = (double)step / samples;

        MPI_Gather(local_positions, local_n, MPI_DOUBLE,
                   global_positions, local_n, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);

        if (rank == 0)
        {
            print_positions(out_file, step, time, global_positions, points);
        }

	double left_in = 0.0;
	double right_out = local_positions[local_n - 1];

	if (rank == 0)
	{
    		if (size > 1)
    		{
        		MPI_Send(&right_out, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    		}
	}
	else
	{
    		MPI_Recv(&left_in, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    		if (rank < size - 1)
    		{
        		MPI_Send(&right_out, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
    		}
	}

        for (int i = local_n - 1; i > 0; i--)
        {
            local_positions[i] = local_positions[i - 1];
        }

        if (rank == 0)
        {
            local_positions[0] = sin(2.0 * M_PI * time);
        }
        else
        {
            local_positions[0] = left_in;
        }
    }

    if (rank == 0)
    {
        fclose(out_file);
        free(global_positions);
    }

    free(local_positions);
    MPI_Finalize();
    return 0;
}

Args check_args(int argc, char **argv, int rank)
{
    Args args;

    if (rank == 0)
    {
        if (argc != 5)
        {
            fprintf(stderr, "Usage: %s [POINTS] [CYCLES] [SAMPLES_PER_CYCLE] [OUTPUT_CSV]\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, -1);
        }

        args.points = atoi(argv[1]);
        args.cycles = atoi(argv[2]);
        args.samples = atoi(argv[3]);
        snprintf(args.output_path, sizeof(args.output_path), "%s", argv[4]);
    }

    return args;
}

void print_header(FILE *out_file, int points)
{
    fprintf(out_file, "step,time");
    for (int i = 0; i < points; i++)
    {
        fprintf(out_file, ",y%d", i);
    }
    fprintf(out_file, "\n");
}

void initialise_positions(double positions[], int n)
{
    for (int i = 0; i < n; i++)
    {
        positions[i] = 0.0;
    }
}

void print_positions(FILE *out_file, int step, double time, double positions[], int points)
{
    fprintf(out_file, "%d,%f", step, time);
    for (int i = 0; i < points; i++)
    {
        fprintf(out_file, ",%f", positions[i]);
    }
    fprintf(out_file, "\n");
}
