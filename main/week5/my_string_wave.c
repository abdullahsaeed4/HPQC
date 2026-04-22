#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int points;
    int cycles;
    int samples;
    const char *output_path;
} Args;

Args check_args(int argc, char **argv);
void print_header(FILE *out_file, int points);
void initialise_positions(double positions[], int points);
void update_positions(double positions[], int points, double time);
void print_positions(FILE *out_file, int step, double time, double positions[], int points);

int main(int argc, char **argv)
{
    Args args = check_args(argc, argv);

    int points = args.points;
    int cycles = args.cycles;
    int samples = args.samples;
    const char *output_path = args.output_path;

    double *positions = malloc(points * sizeof(double));
    if (positions == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    FILE *out_file = fopen(output_path, "w");
    if (out_file == NULL)
    {
        fprintf(stderr, "Error: could not open output file %s\n", output_path);
        free(positions);
        return -1;
    }

    initialise_positions(positions, points);
    print_header(out_file, points);

    int total_steps = cycles * samples;
    for (int step = 0; step <= total_steps; step++)
    {
        double time = (double)step / samples;
        print_positions(out_file, step, time, positions, points);
        update_positions(positions, points, time);
    }

    fclose(out_file);
    free(positions);
    return 0;
}

Args check_args(int argc, char **argv)
{
    Args args;

    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s [POINTS] [CYCLES] [SAMPLES_PER_CYCLE] [OUTPUT_CSV]\n", argv[0]);
        exit(-1);
    }

    args.points = atoi(argv[1]);
    args.cycles = atoi(argv[2]);
    args.samples = atoi(argv[3]);
    args.output_path = argv[4];

    if (args.points < 2 || args.cycles < 1 || args.samples < 1)
    {
        fprintf(stderr, "Error: invalid numerical arguments\n");
        exit(-1);
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

void initialise_positions(double positions[], int points)
{
    for (int i = 0; i < points; i++)
    {
        positions[i] = 0.0;
    }
}

void update_positions(double positions[], int points, double time)
{
    for (int i = points - 1; i > 0; i--)
    {
        positions[i] = positions[i - 1];
    }

    positions[0] = sin(2.0 * M_PI * time);
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

