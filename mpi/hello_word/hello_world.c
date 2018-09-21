#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(int argc, char **argv)
{
    int comm_size;
    int my_rank;
    char greeting[MAX_STRING];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if (my_rank != 0)
    {
        sprintf(greeting, "greeting from process %d of %d!", my_rank, comm_size);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        printf("Greeting from process %d of %d!\n", my_rank, comm_size);
        for (int i = 1; i < comm_size; i++)
        {
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", greeting);
        }
    }
    MPI_Finalize();
    exit(0);
}