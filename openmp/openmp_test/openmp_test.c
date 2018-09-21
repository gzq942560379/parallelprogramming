#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Hello(void);

int main(int argc, char **argv)
{
    int thread_count = 4;
#pragma omp parallel num_threads(thread_count)
    Hello();

    exit(0);
}

void Hello(void)
{
    int total_num = omp_get_num_threads();
    int my_rank = omp_get_thread_num();
    printf("hello world !!! %d in %d \n", my_rank, total_num);
}