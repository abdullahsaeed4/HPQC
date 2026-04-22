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
void initialise_arrays(double pos[], double vel[], double acc[], int points);
void update_system(double pos[], double vel[], double acc[], int points, double time, double dt, double k, double m);
void print_positions(FILE *out_file, int step, double time, double positions[], int points);

int main(int argc, char **argv)
{
    Args args = check_args(argc, argv);

    int points = args.points;
    int cycles = args.cycles;
    int samples = args.samples;
    const char *output_path = args.output_path;

    double *pos = malloc(points * sizeof(double));
    double *vel = malloc(points * sizeof(double));
    double *acc = malloc(points * sizeof(double));

    if (pos == NULL || vel == NULL || acc == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    FILE *out_file = fopen(output_path, "w");
    if (out_file == NULL)
    {
        fprintf(stderr, "Could not open output file %s\n", output_path);
        free(pos); free(vel); free(acc);
        return -1;
    }

    initialise_arrays(pos, vel, acc, points);
    print_header(out_file, points);

    int total_steps = cycles * samples;
    double dt = 1.0 / samples;
    double k = 20.0;
    double mass = 1.0;

    for (int step = 0; step <= total_steps; step++)
    {
        double time = (double)step / samples;
        print_positions(out_file, step, time, pos, points);
        update_system(pos, vel, acc, points, time, dt, k, mass);
    }

    fclose(out_file);
    free(pos);
    free(vel);
    free(acc);
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

void initialise_arrays(double pos[], double vel[], double acc[], int points)
{
    for (int i = 0; i < points; i++)
    {
        pos[i] = 0.0;
        vel[i] = 0.0;
        acc[i] = 0.0;
    }
}

void update_system(double pos[], double vel[], double acc[], int points, double time, double dt, double k, double m)
{
    double *new_acc = malloc(points * sizeof(double));
    if (new_acc == NULL) return;

    pos[0] = sin(2.0 * M_PI * time);
    vel[0] = 0.0;
    new_acc[0] = 0.0;

    for (int i = 1; i < points - 1; i++)
    {
        double force = k * (pos[i - 1] - 2.0 * pos[i] + pos[i + 1]);
        new_acc[i] = force / m;
    }

    new_acc[points - 1] = 0.0;

    for (int i = 1; i < points; i++)
    {
        vel[i] += new_acc[i] * dt;
        pos[i] += vel[i] * dt;
        acc[i] = new_acc[i];
    }

    free(new_acc);
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
