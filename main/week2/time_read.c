#include <stdio.h>
#include <time.h>

int main(){

    clock_t start = clock();

    FILE *f = fopen("../../data/numbers.txt","r");

    int num;

    while(fscanf(f,"%d",&num)!=EOF){
    }

    fclose(f);

    clock_t end = clock();

    double time_taken = (double)(end-start)/CLOCKS_PER_SEC;

    printf("Read time: %f seconds\n",time_taken);

    return 0;
}
