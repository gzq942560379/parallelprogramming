#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <omp.h>
#include <errno.h>

int main(int argc, char **argv)
{

    int thread_count = 4;
    int n = 7;
    int arr[] = {3, 3, 4, 5, 6, 1, 2};

    int i, j, count;
    int *tmp = malloc(sizeof(int) * n);
#pragma omp parallel for num_threads(thread_count) default(none) shared(arr, n, tmp) private(i, j, count)
    for (i = 0; i < n; i++)
    {
        count = 0;
        for (j = 0; j < n; j++)
        {
            if (arr[j] < arr[i] || (arr[j] == arr[i] && j < i))
            {
                count++;
            }
        }
        tmp[count] = arr[i];
    }
    memcpy(arr, tmp, n * sizeof(int));
    free(tmp);

    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    exit(0);
}