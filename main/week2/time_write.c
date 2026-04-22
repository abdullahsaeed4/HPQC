#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);

    clock_t start = clock();

    FILE *f = fopen("../../data/numbers.txt","w");

    for(int i=0;i<n;i++){
        fprintf(f,"%d\n",i);
    }

    fclose(f);

    clock_t end = clock();

    double time_taken = (double)(end-start)/CLOCKS_PER_SEC;

    printf("Write time: %f seconds\n",time_taken);

    return 0;
}
