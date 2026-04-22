#include <stdio.h>
#include <stdlib.h>

int check_args(int argc, char **argv);
int sum_vector(int vector[], int size);

int main(int argc, char **argv)
{
    int num_arg = check_args(argc, argv);

    int *my_vector = malloc(num_arg * sizeof(int));
    if (my_vector == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    for (int i = 0; i < num_arg; i++)
    {
        my_vector[i] = i * i;
    }

    int my_sum = sum_vector(my_vector, num_arg);

    printf("Sum: %d\n", my_sum);

    free(my_vector);
    return 0;
}

int sum_vector(int vector[], int size)
{
    int sum = 0;

    for (int i = 0; i < size; i++)
    {
        sum += vector[i];
    }

    return sum;
}

int check_args(int argc, char **argv)
{
    int num_arg = 0;

    if (argc == 2)
    {
        num_arg = atoi(argv[1]);
    }
    else
    {
        fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
        fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);
        exit(-1);
    }

    return num_arg;
}
